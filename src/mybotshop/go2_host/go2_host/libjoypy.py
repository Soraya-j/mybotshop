#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Software License Agreement (BSD)
#
# @author    Salman Omar Sohail <support@mybotshop.de>
# @copyright (c) 2024, MYBOTSHOP GmbH, Inc., All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# * Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# * Neither the name of MYBOTSHOP GmbH nor the names of its contributors
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# Redistribution and use in source and binary forms, with or without
# modification, is not permitted without the express permission
# of MYBOTSHOP GmbH.

import os
import time
import paramiko
from rclpy.node import Node
from sensor_msgs.msg import Joy

# Mode is Turned Off & State is X
A_BUT = 0
B_BUT = 1
X_BUT = 2
Y_BUT = 3
R1_BUT = 5
L1_BUT = 4
BACK_BUT = 6
START_BUT = 7
LJ_BUT = 9
RJ_BUT = 10

LE_AX = 6
RI_AX = 6  # -ve
UP_AX = 7
DO_AX = 7  # -ve
L2_AX = 2
R2_AX = 5


class Logitechf710Controls(Node):

    def __init__(self):
        super().__init__('host_logitech_f710')
        self.nsp = os.environ.get('GO2_NS', 'go2_unit_0001')

        # Parameters
        self.declare_parameters(namespace='', parameters=[
            ('remote_user', None),
            ('remote_host', None),
            ('remote_host_pswd', None),

        ])

        self.param_remote_user = self.get_parameter('remote_user').value
        self.param_remote_host = self.get_parameter('remote_host').value
        self.param_remote_host_pswd = self.get_parameter(
            'remote_host_pswd').value
        
        self.get_logger().info(
            f'{self.colorize("-> GO2 LogitechF710 Host Controller Parameters","cyan")}')
        self.get_logger().info(
            f'{self.colorize("remote_user:","blue")} {self.param_remote_user}')
        self.get_logger().info(
            f'{self.colorize("remote_ip:","blue")} {self.param_remote_host}')
        self.get_logger().info(
            f'{self.colorize("remote_ip_pswd:","blue")} {self.param_remote_host_pswd}')

        self.print_mode_activation()

        # Subscribe to the joystick topic
        self.subscription = self.create_subscription(
            Joy, 'joy', self.joy_callback, 10)

        # Cooldown period
        self.cooldown = 1.0
        self.last_activation_time = time.time()

        # SSH Session
        try:
            self.setup_persistent_ssh()
        except Exception as e:
            self.get_logger().error(self.colorize(
                f"Error in SSH Connection: {e}", "red"))

    def print_mode_activation(self):
        self.get_logger().info(
            f'{self.colorize("","cyan")}')
        self.get_logger().info(
            f'{self.colorize("-> GO2 LogitechF710 Host Controls","cyan")}')
        self.get_logger().info(
            f'{self.colorize("- R2 + START: Reset SSH Server","orange")}')
        self.get_logger().info(
            f'{self.colorize("- R2 + A: Sports Mode","orange")}')
        self.get_logger().info(
            f'{self.colorize("- R2 + UP: Stand Up","orange")}')
        self.get_logger().info(
            f'{self.colorize("- R2 + DOWN: Stand Down","orange")}')
        self.get_logger().info(
            f'{self.colorize("- L2 + B: Gripper Close","orange")}')
        self.get_logger().info(
            f'{self.colorize("- L2 + X: Gripper Open","orange")}')
        self.get_logger().info(
            f'{self.colorize("- L2 + START: Home Pose","orange")}')
        self.get_logger().info(
            f'{self.colorize("- L2 + RIGHT: Transport Pose","orange")}')
        self.get_logger().info(
            f'{self.colorize("- L2 + UP: Stand Pose","orange")}')

    def setup_persistent_ssh(self):
        """Establish a persistent SSH connection using ControlMaster"""
        self.ssh_client = paramiko.SSHClient()
        self.ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        self.ssh_client.connect(hostname=self.param_remote_host,
                                username=self.param_remote_user,
                                password=self.param_remote_host_pswd)
        self.shell = self.ssh_client.invoke_shell()

        self.shell.send('export ROS_DOMAIN_ID=10\n')
        time.sleep(0.1)
        self.shell.send('source /opt/ros/foxy/setup.bash\n')
        time.sleep(0.1)
        self.shell.send(
            f'source /home/{self.param_remote_user}/ros2_ws/src/go2_bringup/config/setup.bash\n')
        time.sleep(0.1)

        self.get_logger().info(
            f'{self.colorize("SSH connection established","purple")}')

    def joy_callback(self, msg):

        self.get_logger().info(
            f'{self.colorize("GO2 Logitech F710 Joystick Activated","green")}', once=True)

        try:
            current_time = time.time()

            # Check if cooldown period has passed
            if current_time - self.last_activation_time < self.cooldown:
                return  # Skip processing if still in cooldown

            if msg.buttons[START_BUT] == 1 and msg.axes[R2_AX] == -1.0:
                self.get_logger().info(
                    f'{self.colorize("Reset SSH Server","yellow")}')
                self.setup_persistent_ssh()
                self.last_activation_time = current_time
                return
            # --------------------------------------- GO2 Robot Controls --------------------------------------- #
            # Robot sports mode
            if msg.buttons[A_BUT] == 1 and msg.axes[R2_AX] == -1.0:
                self.get_logger().info(
                    f'{self.colorize("Logitech F710 GO2 Sports Mode","yellow")}')
                self.send_go2_mode_command('sports_mode')
                self.last_activation_time = current_time
                return

            # Robot recovery_stand Up pad
            if msg.axes[UP_AX] == 1.0 and msg.axes[R2_AX] == -1.0:
                self.get_logger().info(
                    f'{self.colorize("Logitech F710 GO2 Stand Up","yellow")}')
                self.send_go2_mode_command('recovery_stand')
                self.last_activation_time = current_time
                return

            # Robot stand_down Down pad
            if msg.axes[DO_AX] == -1.0 and msg.axes[R2_AX] == -1.0:
                self.get_logger().info(
                    f'{self.colorize("Logitech F710 GO2 Stand Down","yellow")}')
                self.send_go2_mode_command('stand_down')
                self.last_activation_time = current_time
                return

            # --------------------------------------- Open Manipulator Controls --------------------------------------- #
            # Open Manipulator Gripper Controls Close
            if msg.buttons[B_BUT] == 1:
                self.get_logger().info(
                    f'{self.colorize("Open Manipulator Gripper Close","orange")}')
                self.send_manipulator_command('g_close')
                self.last_activation_time = current_time
                return

            # Open Manipulator Gripper Controls Open
            if msg.buttons[X_BUT] == 1:
                self.get_logger().info(
                    f'{self.colorize("Open Manipulator Gripper Open","orange")}')
                self.send_manipulator_command('g_open')
                self.last_activation_time = current_time
                return

            # Move open manipulator to home pose
            if msg.axes[L2_AX] == -1.0 and msg.buttons[START_BUT] == 1:
                self.get_logger().info(
                    f'{self.colorize("Open Manipulator Home Pose","orange")}')
                self.send_manipulator_command('home')
                self.last_activation_time = current_time
                return

            # Move open manipulator to transport pose
            if msg.axes[L2_AX] == -1.0 and msg.axes[RI_AX] == -1.0:
                self.get_logger().info(
                    f'{self.colorize("Open Manipulator Transport Pose","orange")}')
                self.send_manipulator_command('transport')
                self.last_activation_time = current_time
                return

            # Move open manipulator to stand pose
            if msg.axes[L2_AX] == -1.0 and msg.axes[UP_AX] == 1.0:
                self.get_logger().info(
                    f'{self.colorize("Open Manipulator Stand Pose","orange")}')
                self.send_manipulator_command('stand')
                self.last_activation_time = current_time
                return

        except Exception as e:
            self.get_logger().error(
                f"Exception occurred in joystick callback: {e}")

    def send_go2_mode_command(self, mode):
        command = f"ros2 service call /{self.nsp}/modes go2_interface/srv/Go2Modes \"{{request_data: '{mode}'}}\""
        self.execute_ros2_command_remotely(command)

    def send_manipulator_command(self, pose):
        command = f"ros2 service call /{self.nsp}/moveit2/predefined_pose go2_interface/srv/Go2Modes \"{{request_data: '{pose}'}}\""
        self.execute_ros2_command_remotely(command)

    def execute_ros2_command_remotely(self, command):
        '''
        Execute ROS2 command on the remote machine using persistent connection
        '''
        # self.get_logger().info(
        #     f'{self.colorize(f"Executing Command: {command}","blue")}')
        self.shell.send(command+'\n')
        time.sleep(1)
        output = self.shell.recv(9999)
        try:
            op = output.decode('utf-8').replace('\x00',
                                                '')  # Remove null chars
        except UnicodeDecodeError as e:
            self.get_logger().error(f'Decode error: {e}')
            return
        # self.get_logger().info(
        #     f'{self.colorize(f"Command Output: {op}","blue")}')

    def colorize(self, text, color):
        color_codes = {
            'green': '\033[92m',
            'yellow': '\033[93m',
            'orange': '\033[38;5;208m',
            'blue': '\033[94m',
            'red': '\033[91m',
            'purple': '\033[95m',
            'cyan': '\033[96m',
        }
        return color_codes[color] + text + '\033[0m'
