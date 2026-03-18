#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
==============================================================================
GO2-W Stand and Sit Demo - Low-Level Motor Control Example
==============================================================================

This script demonstrates LOW-LEVEL motor control on the Unitree GO2-W robot.
It bypasses the built-in motion controller and sends direct commands to each
motor using PD (Proportional-Derivative) position control.

Based on the official Unitree SDK2 example:
    src/third_party/unitree/20Jan2026_unitree_sdk2_python/example/go2w/low_level/go2w_stand_example.py

WHAT THIS SCRIPT DOES:
    1. Connects to the robot via DDS (Data Distribution Service)
    2. Disables the high-level sport mode controller
    3. Reads current motor positions
    4. Smoothly interpolates joints to standing position
    5. Holds standing for a few seconds
    6. Smoothly interpolates joints back to sitting position

KEY CONCEPTS DEMONSTRATED:
    - Direct motor command publishing (LowCmd)
    - Motor state subscription (LowState)
    - PD position control (Kp, Kd gains)
    - Smooth trajectory interpolation
    - 500 Hz control loop timing

MOTOR LAYOUT (GO2-W has 16 motors):
    Leg joints (12 motors, indices 0-11):
        FR (Front Right): hip=0, thigh=1, calf=2
        FL (Front Left):  hip=3, thigh=4, calf=5
        RR (Rear Right):  hip=6, thigh=7, calf=8
        RL (Rear Left):   hip=9, thigh=10, calf=11

    Wheels (4 motors, indices 12-15):
        FR_wheel=12, FL_wheel=13, RR_wheel=14, RL_wheel=15

Usage:
    ros2 launch go2w_actuator_control go2w_actuator.launch.py
    python3 go2w_stand_sit.py [network_interface]
"""

import time
import sys
import logging
import termios
import tty
import select

# ==============================================================================
# LOGGING SETUP (real-time output)
# ==============================================================================
logging.basicConfig(
    level=logging.INFO,
    format='[%(levelname)s] [%(name)s]: %(message)s',
    stream=sys.stdout
)
sys.stdout.reconfigure(line_buffering=True)
logger = logging.getLogger('go2w_stand_sit')


def get_key(timeout=0.1):
    """Get a single keypress without blocking."""
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(fd)
        rlist, _, _ = select.select([sys.stdin], [], [], timeout)
        if rlist:
            return sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return None

# ==============================================================================
# UNITREE SDK2 IMPORTS
# ==============================================================================
from unitree_sdk2py.core.channel import (
    ChannelPublisher,
    ChannelSubscriber,
    ChannelFactoryInitialize
)
from unitree_sdk2py.idl.default import unitree_go_msg_dds__LowCmd_
from unitree_sdk2py.idl.unitree_go.msg.dds_ import LowCmd_, LowState_
from unitree_sdk2py.utils.crc import CRC
from unitree_sdk2py.utils.thread import RecurrentThread
from unitree_sdk2py.comm.motion_switcher.motion_switcher_client import MotionSwitcherClient
from unitree_sdk2py.go2.sport.sport_client import SportClient


# ==============================================================================
# CONSTANTS
# ==============================================================================

LOWLEVEL = 0xFF
PosStopF = 2.146e9
VelStopF = 16000.0

TOPIC_LOWCMD = "rt/lowcmd"
TOPIC_LOWSTATE = "rt/lowstate"

NUM_LEG_JOINTS = 12
MOTOR_ARRAY_SIZE = 20
CONTROL_DT = 0.002  # 500 Hz

# PD Control Gains (increased for stiffer joints)
Kp = 100.0
Kd = 8.0

# Wheel indices
WHEEL_FR, WHEEL_FL, WHEEL_RR, WHEEL_RL = 12, 13, 14, 15
WHEEL_SPEED = 2.0  # rad/s

# ==============================================================================
# TARGET POSITIONS (radians)
# ==============================================================================

STAND_POS = [
    0.0, 0.67, -1.3,   # FR
    0.0, 0.67, -1.3,   # FL
    0.0, 0.67, -1.3,   # RR
    0.0, 0.67, -1.3    # RL
]

SIT_POS = [
    0.0, 1.36, -2.65,  # FR
    0.0, 1.36, -2.65,  # FL
    0.0, 1.36, -2.65,  # RR
    0.0, 1.36, -2.65   # RL
]


# ==============================================================================
# CONTROLLER CLASS
# ==============================================================================

class StandSitController:
    """Low-level controller for stand/sit demonstration."""

    def __init__(self):
        self.dt = CONTROL_DT

        self.low_cmd = unitree_go_msg_dds__LowCmd_()
        self.low_state = None

        self.start_pos = [0.0] * NUM_LEG_JOINTS
        self.first_run = True
        self.done = False

        self.percent_1 = 0.0
        self.percent_2 = 0.0
        self.percent_3 = 0.0

        # Phase durations: 5 seconds each (at 500 Hz)
        self.duration_1 = 2500  # 5 seconds to stand
        self.duration_2 = 2500  # 5 seconds to hold
        self.duration_3 = 2500  # 5 seconds to sit

        # Wheel velocities [FR, FL, RR, RL]
        self.wheel_vel = [0.0, 0.0, 0.0, 0.0]
        self.teleop_mode = False

        self.crc = CRC()

        self.lowcmd_publisher = None
        self.lowstate_subscriber = None
        self.control_thread = None

    def Init(self):
        """Initialize communication and release high-level control."""
        self._init_low_cmd()

        self.lowcmd_publisher = ChannelPublisher(TOPIC_LOWCMD, LowCmd_)
        self.lowcmd_publisher.Init()

        self.lowstate_subscriber = ChannelSubscriber(TOPIC_LOWSTATE, LowState_)
        self.lowstate_subscriber.Init(self._low_state_handler, 10)

        self._release_high_level_control()

        logger.info("Waiting for robot state...")
        timeout = 50
        while self.low_state is None and timeout > 0:
            time.sleep(0.1)
            timeout -= 1

        if self.low_state is None:
            logger.error("No state received from robot!")
            sys.exit(1)

        logger.info("Robot state received. Ready.")

    def Start(self):
        """Start the control loop."""
        logger.info("Starting 500 Hz control loop...")
        self.control_thread = RecurrentThread(
            interval=self.dt,
            target=self._control_loop,
            name="stand_sit_control"
        )
        self.control_thread.Start()

    def _init_low_cmd(self):
        """Initialize low command with safe defaults."""
        self.low_cmd.head[0] = 0xFE
        self.low_cmd.head[1] = 0xEF
        self.low_cmd.level_flag = LOWLEVEL
        self.low_cmd.gpio = 0

        for i in range(MOTOR_ARRAY_SIZE):
            self.low_cmd.motor_cmd[i].mode = 0x01
            self.low_cmd.motor_cmd[i].q = PosStopF
            self.low_cmd.motor_cmd[i].kp = 0
            self.low_cmd.motor_cmd[i].dq = VelStopF
            self.low_cmd.motor_cmd[i].kd = 0
            self.low_cmd.motor_cmd[i].tau = 0

    def _low_state_handler(self, msg: LowState_):
        """Callback for state messages."""
        self.low_state = msg

    def _release_high_level_control(self):
        """Release high-level sport mode."""
        logger.info("Releasing high-level control...")

        sc = SportClient()
        sc.SetTimeout(5.0)
        sc.Init()

        msc = MotionSwitcherClient()
        msc.SetTimeout(5.0)
        msc.Init()

        status, result = msc.CheckMode()
        while result['name']:
            logger.info(f"Releasing mode: {result['name']}")
            sc.StandUp()
            sc.StandDown()
            msc.ReleaseMode()
            status, result = msc.CheckMode()
            time.sleep(1)

        logger.info("High-level control released.")

    def _control_loop(self):
        """Main control loop at 500 Hz."""
        if self.low_state is None:
            return

        if self.first_run:
            logger.info("Recording initial joint positions...")
            for i in range(NUM_LEG_JOINTS):
                self.start_pos[i] = self.low_state.motor_state[i].q
            self.first_run = False
            logger.info("Phase 1: Standing up...")

        # PHASE 1: Stand up
        if self.percent_1 < 1.0:
            self.percent_1 += 1.0 / self.duration_1
            self.percent_1 = min(self.percent_1, 1.0)

            for i in range(NUM_LEG_JOINTS):
                self.low_cmd.motor_cmd[i].q = (
                    (1 - self.percent_1) * self.start_pos[i] +
                    self.percent_1 * STAND_POS[i]
                )
                self.low_cmd.motor_cmd[i].dq = 0
                self.low_cmd.motor_cmd[i].kp = Kp
                self.low_cmd.motor_cmd[i].kd = Kd
                self.low_cmd.motor_cmd[i].tau = 0

        # PHASE 2: Hold standing (stay here during teleop)
        elif self.percent_2 < 1.0 or self.teleop_mode:
            if self.percent_2 == 0:
                logger.info("Phase 2: Holding position...")
                self.percent_2 = 0.01  # Mark as started

            for i in range(NUM_LEG_JOINTS):
                self.low_cmd.motor_cmd[i].q = STAND_POS[i]
                self.low_cmd.motor_cmd[i].dq = 0
                self.low_cmd.motor_cmd[i].kp = Kp
                self.low_cmd.motor_cmd[i].kd = Kd
                self.low_cmd.motor_cmd[i].tau = 0

        # PHASE 3: Sit down
        elif self.percent_3 < 1.0:
            if self.percent_3 == 0:
                logger.info("Phase 3: Sitting down...")

            self.percent_3 += 1.0 / self.duration_3
            self.percent_3 = min(self.percent_3, 1.0)

            for i in range(NUM_LEG_JOINTS):
                self.low_cmd.motor_cmd[i].q = (
                    (1 - self.percent_3) * STAND_POS[i] +
                    self.percent_3 * SIT_POS[i]
                )
                self.low_cmd.motor_cmd[i].dq = 0
                self.low_cmd.motor_cmd[i].kp = Kp
                self.low_cmd.motor_cmd[i].kd = Kd
                self.low_cmd.motor_cmd[i].tau = 0

        # DONE: Hold sit position
        else:
            if not self.done:
                logger.info("Sequence complete!")
                self.done = True

            for i in range(NUM_LEG_JOINTS):
                self.low_cmd.motor_cmd[i].q = SIT_POS[i]
                self.low_cmd.motor_cmd[i].dq = 0
                self.low_cmd.motor_cmd[i].kp = Kp
                self.low_cmd.motor_cmd[i].kd = Kd
                self.low_cmd.motor_cmd[i].tau = 0

        # Apply wheel velocities (velocity control mode: kp=0)
        for i, idx in enumerate([WHEEL_FR, WHEEL_FL, WHEEL_RR, WHEEL_RL]):
            self.low_cmd.motor_cmd[idx].q = 0
            self.low_cmd.motor_cmd[idx].kp = 0
            self.low_cmd.motor_cmd[idx].dq = self.wheel_vel[i]
            self.low_cmd.motor_cmd[idx].kd = Kd
            self.low_cmd.motor_cmd[idx].tau = 0

        # Send command
        self.low_cmd.crc = self.crc.Crc(self.low_cmd)
        self.lowcmd_publisher.Write(self.low_cmd)


# ==============================================================================
# MAIN
# ==============================================================================

if __name__ == '__main__':
    logger.info("=" * 50)
    logger.info("GO2-W LOW-LEVEL CONTROL DEMO: Stand & Sit")
    logger.info("=" * 50)
    logger.info("WARNING: Ensure robot is on the ground!")

    logger.info("Press ENTER to start...")
    input()

    # Initialize DDS
    if len(sys.argv) > 1:
        ChannelFactoryInitialize(0, sys.argv[1])
    else:
        ChannelFactoryInitialize(0)

    controller = StandSitController()
    controller.Init()
    controller.Start()

    # Wait for stand-up to complete
    while controller.percent_1 < 1.0:
        time.sleep(0.1)

    controller.teleop_mode = True
    logger.info("Teleop: i=fwd, ,=back, k=stop, j=left, l=right, q=quit")

    try:
        while True:
            key = get_key(0.1)
            if key == 'i':
                controller.wheel_vel = [WHEEL_SPEED] * 4
            elif key == ',':
                controller.wheel_vel = [-WHEEL_SPEED] * 4
            elif key == 'k':
                controller.wheel_vel = [0.0] * 4
            elif key == 'j':
                controller.wheel_vel = [-WHEEL_SPEED, WHEEL_SPEED, -WHEEL_SPEED, WHEEL_SPEED]
            elif key == 'l':
                controller.wheel_vel = [WHEEL_SPEED, -WHEEL_SPEED, WHEEL_SPEED, -WHEEL_SPEED]
            elif key == 'q':
                break
    except KeyboardInterrupt:
        pass

    # Stop wheels and sit down
    logger.info("Sitting down...")
    controller.wheel_vel = [0.0] * 4
    controller.teleop_mode = False  # Exit teleop mode to allow Phase 3
    controller.percent_2 = 1.0      # Skip hold phase, go to sit
    while controller.percent_3 < 1.0:
        time.sleep(0.1)
    logger.info("Done!")
    time.sleep(0.5)
    sys.exit(0)
