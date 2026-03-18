# GO2-W Actuator Control

Low-level actuator control demo for the Unitree GO2-W wheeled quadruped robot.

## Overview

This package demonstrates **direct motor control** using the Unitree SDK2, bypassing the robot's built-in motion controller. This gives full control over each joint and wheel motor.

## How It Works

### 1. Release High-Level Control

Before sending low-level commands, we must release the robot's built-in sport mode controller:

```
SportClient.StandUp()      # Ensure robot is in known state
SportClient.StandDown()    # Put robot in rest position
MotionSwitcherClient.ReleaseMode()   # Release high-level control
```

This hands over motor control from the internal controller to our script.

### 2. DDS Communication

The script communicates with the robot via DDS (Data Distribution Service):

| Topic | Type | Purpose |
|-------|------|---------|
| `rt/lowcmd` | LowCmd_ | Send motor commands |
| `rt/lowstate` | LowState_ | Receive motor feedback |

### 3. Motor Layout

GO2-W has **16 motors** total:

```
Leg Joints (12 motors, indices 0-11):
    Front Right (FR): hip=0,  thigh=1,  calf=2
    Front Left  (FL): hip=3,  thigh=4,  calf=5
    Rear Right  (RR): hip=6,  thigh=7,  calf=8
    Rear Left   (RL): hip=9,  thigh=10, calf=11

Wheels (4 motors, indices 12-15):
    FR_wheel=12, FL_wheel=13, RR_wheel=14, RL_wheel=15
```

### 4. PD Position Control

Each motor is controlled using PD (Proportional-Derivative) control:

```
torque = Kp * (target_pos - current_pos) + Kd * (target_vel - current_vel) + feedforward_torque
```

| Parameter | Description | Value |
|-----------|-------------|-------|
| `q` | Target position (rad) | Joint angle |
| `dq` | Target velocity (rad/s) | 0 for position hold |
| `kp` | Position gain | 100.0 (stiffer = higher) |
| `kd` | Velocity gain | 8.0 (damping) |
| `tau` | Feedforward torque | 0 |

**Wheel velocity control**: Set `kp=0` to disable position control, then `dq` becomes the velocity setpoint.

### 5. Control Loop

The script runs at **500 Hz** (0.002s interval):

1. Read current motor states
2. Compute target positions (interpolated for smooth motion)
3. Apply PD gains
4. Compute CRC checksum
5. Publish command

## Usage

### Launch with ROS2

```bash
ros2 launch go2w_actuator_control go2w_actuator.launch.py
```

### Run Standalone

```bash
cd src/mybotshop/go2w_actuator_control/scripts
python3 go2w_stand_sit.py [network_interface]
```

### Controls

| Key | Action |
|-----|--------|
| `Enter` | Start (robot stands up) |
| `i` | Move forward |
| `,` | Move backward |
| `k` | Stop wheels |
| `j` | Turn left |
| `l` | Turn right |
| `q` | Quit (robot sits down) |

## Key Parameters

| Parameter | Value | Description |
|-----------|-------|-------------|
| `ROS_DOMAIN_ID` | 10 | DDS domain for robot communication |
| `CONTROL_DT` | 0.002s | Control loop period (500 Hz) |
| `Kp` | 100.0 | Joint stiffness |
| `Kd` | 8.0 | Joint damping |
| `WHEEL_SPEED` | 2.0 rad/s | Teleop wheel velocity |

## Safety Notes

- Ensure robot is on the ground before running
- Keep clear of the robot during operation
- Press `q` or `Ctrl+C` to safely stop and sit down
- The script will hold the sitting position after completion

---

## RL SAR Simulation (Gazebo)

For RL-based control simulation, use the [rl_sar](https://github.com/fan-ziqi/rl_sar) framework.

### Setup

```bash
cd ~/projects/quadruped/rl_sar
git submodule update --init --recursive
sudo apt install liblcm-dev
./build.sh
```

### Launch Simulation

**Terminal 1 - Gazebo:**
```bash
source /usr/share/gazebo-11/setup.sh
source ~/projects/quadruped/rl_sar/install/setup.bash
ros2 launch rl_sar gazebo.launch.py rname:=go2w
```

**Terminal 2 - RL Controller:**
```bash
source ~/projects/quadruped/rl_sar/install/setup.bash
ros2 run rl_sar rl_sim
```

### Control with Gamepad or Keyboard

#### Basic Controls

| Gamepad | Keyboard | Description |
|---------|----------|-------------|
| A | Num0 | Move robot from initial pose to `default_dof_pos` (position control interpolation) |
| B | Num9 | Move robot from current position to initial pose |
| X | N | Toggle navigation mode (disables velocity commands, receives `cmd_vel` topic) |
| Y | N/A | N/A |

#### Simulation Controls

| Gamepad | Keyboard | Description |
|---------|----------|-------------|
| RB+Y | R | Reset Gazebo environment (stand up fallen robot) |
| RB+X | Enter | Toggle Gazebo run/stop (default: running) |

#### Motor Controls

| Gamepad | Keyboard | Description |
|---------|----------|-------------|
| LB+A | M | Motor enable (recommended) |
| LB+B | K | Motor disable (recommended) |
| LB+X | P | Motor passive mode (kp=0, kd=8) |
| LB+RB | N/A | Emergency stop (recommended) |

#### Skill Selection

| Gamepad | Keyboard | Description |
|---------|----------|-------------|
| RB+DPad Up | Num1 | Basic Locomotion |
| RB+DPad Down | Num2 | Skill 2 |
| RB+DPad Left | Num3 | Skill 3 |
| RB+DPad Right | Num4 | Skill 4 |
| LB+DPad Up | Num5 | Skill 5 |
| LB+DPad Down | Num6 | Skill 6 |
| LB+DPad Left | Num7 | Skill 7 |
| LB+DPad Right | Num8 | Skill 8 |

#### Movement Controls

| Gamepad | Keyboard | Description |
|---------|----------|-------------|
| Left Stick Y | W/S | Forward/Backward (X-axis) |
| Left Stick X | A/D | Left/Right strafe (Y-axis) |
| Right Stick X | Q/E | Yaw rotation |
| Release joystick | Space | Reset all commands to zero |

---

## References

- Based on: `unitree_sdk2_python/example/go2w/low_level/go2w_stand_example.py`
- Unitree SDK2 Python documentation
- [rl_sar](https://github.com/fan-ziqi/rl_sar) - RL simulation and real deployment framework
