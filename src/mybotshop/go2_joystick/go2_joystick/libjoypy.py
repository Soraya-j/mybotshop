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

import time
import rclpy
import subprocess

from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist


class CustomJoyControls:

    def __init__(self):
        rclpy.init()
        self.node = rclpy.create_node('go2_joystick_commands')
        self.subscription = self.node.create_subscription(
            Joy, 'joy', self.joy_callback, 10)
        self.euler_pub = self.node.create_publisher(Twist, 'euler_cmd', 10)
        self.move_pub = self.node.create_publisher(Twist, 'hardware/cmd_vel', 10)
        self.activate = False

    def execute_program(self):
        rclpy.spin(self.node)

    def joy_callback(self, msg):

        self.node.get_logger().info(
            f'{self.colorize("GO2 Logitech F710 Joystick Activated","green")}', once=True)

        if msg.axes[1] == 1.0 and msg.buttons[0] == 1:  # LeftUp + A
            self.node.get_logger().info(
                f'{self.colorize("Logitech F710 GO2 Stand up","yellow")}')
            command = "ros2 service call /go2_unit_49702/modes go2_interface/srv/Go2Modes \"{request_data: 'stand_up'}\""
            self.execute_ros2_command(command)
            time.sleep(3)

        if msg.axes[1] == -1.0 and msg.buttons[0] == 1:  # LeftUp + A
            self.node.get_logger().info(
                f'{self.colorize("Logitech F710 GO2 Stand down","yellow")}')
            command = "ros2 service call /go2_unit_49702/modes go2_interface/srv/Go2Modes \"{request_data: 'stand_down'}\"" 
            self.execute_ros2_command(command)
            time.sleep(2)
        
        if msg.buttons[3] == 1:         # X
            self.node.get_logger().info(
                f'{self.colorize("Euler","orange")}')

            twist = Twist()
            twist.linear.x = msg.axes[1] * 0.5   
            twist.linear.y = msg.axes[0] * 0.5   
            twist.angular.z = msg.axes[2] * 0.5  

            self.euler_pub.publish(twist)
 
        if msg.buttons[7] == 1:         #R (avancer vitesse normale)
            self.node.get_logger().info(f'{self.colorize("Move","orange")}')
            twist = Twist()
            twist.linear.x = msg.axes[1] * 0.5   
            twist.linear.y = msg.axes[0] * 0.5   
            twist.angular.z = msg.axes[2] * 0.5  
            self.move_pub.publish(twist)

        if msg.buttons[1] == 1:         #B
            if self.activate == False:
                self.activate = True
                self.node.get_logger().info(f'{self.colorize("Enter economic walk","orange")}')           
                command = "ros2 service call /go2_unit_49702/modes go2_interface/srv/Go2Modes \"{request_data: 'gait_type_economic'}\""
                self.execute_ros2_command(command)
            else :
                self.activate = False
                self.node.get_logger().info(f'{self.colorize("Enter economic walk","orange")}')           
                command = "ros2 service call /go2_unit_49702/modes go2_interface/srv/Go2Modes \"{request_data: 'gait_type_classic'}\""
                self.execute_ros2_command(command)

    time.sleep(1)
        

        
# ______________________________________________________________________________________________________________________________________________________
# COMMAND FOR THE GRIPPER : 

        # # if msg.axes[5] == -1.0 and msg.buttons[1] == 1: # RT + B
        # #     self.node.get_logger().info(f'{self.colorize("Logitech F710 GO2 Docking Manuever","yellow")}')
        # #     command = "ros2 service call /go2/dock std_srvs/srv/Trigger \"{}\""
        # #     self.execute_ros2_command(command)
        # #     time.sleep(2)

        # if msg.axes[2] == -1.0 and msg.buttons[1] == 1:  # LT + B
        #     self.node.get_logger().info(
        #         f'{self.colorize("Open Manipulator Gripper Close","orange")}')
        #     action_command = "ros2 action send_goal /open_manipulator/gripper_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory -f \"{trajectory: { joint_names: [gripper], points: [ { positions: [1.0], time_from_start: { sec: 0.1 } }, ] }}\""
        #     self.execute_ros2_command(action_command)

        # if msg.axes[2] == -1.0 and msg.buttons[2] == 1:  # LT + X
        #     self.node.get_logger().info(
        #         f'{self.colorize("Open Manipulator Gripper Open","orange")}')
        #     action_command = "ros2 action send_goal /open_manipulator/gripper_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory -f \"{trajectory: { joint_names: [gripper], points: [ { positions: [-1.0], time_from_start: { sec: 0.1 } }, ] }}\""
        #     self.execute_ros2_command(action_command)

        # # Move open manipulator to home pose
        # if msg.axes[2] == -1.0 and msg.axes[7] == -1.0:  # LT + Down
        #     self.node.get_logger().info(
        #         f'{self.colorize("Open Manipulator Home Pose","orange")}')
        #     action_command2 = "ros2 action send_goal /open_manipulator/joint_trajectory_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory -f \"{trajectory: { joint_names: [joint1, joint2, joint3, joint4], points: [ { positions: [0.0, -1.7, 1.4, 1.2], time_from_start: { sec: 1.5} }, ] }}\""
        #     self.execute_ros2_command(action_command2)

        # # Move open manipulator to transport pose
        # if msg.axes[2] == -1.0 and msg.axes[6] == -1.0:  # LT + Right
        #     self.node.get_logger().info(
        #         f'{self.colorize("Open Manipulator Stand Pose","orange")}')
        #     action_command2 = "ros2 action send_goal /open_manipulator/joint_trajectory_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory -f \"{trajectory: { joint_names: [joint1, joint2, joint3, joint4], points: [ { positions: [0.0, -1.169, 1.378, -0.261], time_from_start: { sec: 2 } }, ] }}\""
        #     self.execute_ros2_command(action_command2)
            
        # # Move open manipulator to stand pose
        # if msg.axes[2] == -1.0 and msg.axes[7] == 1.0:  # LT + Up
        #     self.node.get_logger().info(
        #         f'{self.colorize("Open Manipulator Stand Pose","orange")}')
        #     action_command2 = "ros2 action send_goal /open_manipulator/joint_trajectory_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory -f \"{trajectory: { joint_names: [joint1, joint2, joint3, joint4], points: [ { positions: [0.0, -0.575, -0.610, 1.134], time_from_start: { sec: 2 } }, ] }}\""
        #     self.execute_ros2_command(action_command2)

    def execute_ros2_command(self, command):
        try:
            # Execute the command
            process = subprocess.Popen(
                command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            stdout, stderr = process.communicate()
            
            # Check if the command was successful
            if process.returncode == 0:
                self.node.get_logger().info(f'{self.colorize("Command executed successfully:","green")}')
                self.node.get_logger().info(stdout.decode('utf-8'))

            else:
                self.node.get_logger().info(f'{self.colorize("Error executing command:","red")}')
                self.node.get_logger().info(stdout.decode('utf-8'))

        except Exception as e:
            self.node.get_logger().info(f'{self.colorize("Exception occurred: ", "red")}')
            self.node.get_logger().info(e)

    def colorize(self, text, color):
        color_codes = {
            'green': '\033[92m',
            'yellow': '\033[93m',
            'orange': '\033[38;5;208m',
            'blue': '\033[94m',
            'red': '\033[91m'
        }
        return color_codes[color] + text + '\033[0m'
