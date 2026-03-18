#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Software License Agreement (BSD)
#
# @author    Salman Omar Sohail <support@mybotshop.de>
# @copyright (c) 2025, MYBOTSHOP GmbH, Inc., All rights reserved.

import math
from rclpy.node import Node
from go2_zt30.siyi_sdk.siyi_sdk import SIYISDK
from go2_interface.msg import CameraGimbalCmd
from go2_interface.srv import CameraMode
from std_srvs.srv import Trigger
from sensor_msgs.msg import JointState

class ROS2ZT30Server(Node):

    def __init__(self):

        super().__init__('go2_zt30')
        self.get_logger().info(self.colorize("Starting ROS2 SIYI Camera Server", "orange"))

        # Parameters Declaration
        self.declare_parameters(namespace='', parameters=[
            ('zt30_server_ip', '0.0.0.0'),
            ('zt30_server_port', 9900),
        ])

        # Parameters Initialization
        self.param_zt30_server_ip = self.get_parameter('zt30_server_ip').value
        self.param_zt30_server_port = self.get_parameter(
            'zt30_server_port').value

        # Print Parameters
        self.get_logger().info(
            f'{self.colorize(f"zt30_server_ip: {self.param_zt30_server_ip}","blue")}')
        self.get_logger().info(
            f'{self.colorize(f"zt30_server_port: {self.param_zt30_server_port}","blue")}')

        self.initialize_camera()
        
        self.video_flag = False

        # Joint state variables
        self.current_yaw = 0.0
        self.current_pitch = 0.0

        # Joint state publisher
        self.joint_state_pub = self.create_publisher(
            JointState,
            'zt30/joint_states',
            10
        )

        # Gimbal control subscription
        self.create_subscription(
            CameraGimbalCmd,
            'zt30/gimbal/control/cmd',
            self.camera_gimbal_callback,
            10
        )

        # Services
        self.srv_cam_mode = self.create_service(
            CameraMode,
            'zt30/camera_mode',
            self.camera_mode_callback
        )
        
        self.srv_photo = self.create_service(
            Trigger,
            'zt30/trigger_photo',
            self.photo_callback
        )
        
        self.srv_video = self.create_service(
            Trigger,
            'zt30/trigger_video',
            self.video_callback
        )
        
        self.srv_zoomin_control = self.create_service(
            Trigger,
            'zt30/zoomin_control',
            self.zoomin_control_callback
        )

        self.srv_zoomout_control = self.create_service(
            Trigger,
            'zt30/zoomout_control',
            self.zoomout_control_callback
        )

        self.last_gimbal_msg_time = False
        self.gimbal_msg_received = False

        self.create_timer(0.1, self.check_timeout)
        
        # Timer for joint state publishing
        self.create_timer(0.05, self.publish_joint_states)  # 20 Hz

        self.get_logger().info(self.colorize("ROS2 ZT30 Server started", "green"))

    def initialize_camera(self):
        self.cam = SIYISDK(server_ip=self.param_zt30_server_ip,
                           port=self.param_zt30_server_port,
                           debug=False)
        if not self.cam.connect():
            self.get_logger().error(self.colorize("Failed to connect to camera", "red"))
            exit(1)
        self.cam.requestLockMode()
        self.get_logger().info(self.colorize(
            f"Camera Firmware {self.cam.getFirmwareVersion()}", "blue"))
        
    def publish_joint_states(self):
        """Publish current gimbal joint states"""
        try:
            # Get current gimbal attitude from camera (returns tuple: yaw, pitch, roll)
            yaw_deg, pitch_deg, roll_deg = self.cam.getAttitude()
            
            # Convert from degrees to radians
            self.current_yaw = math.radians(yaw_deg)
            self.current_pitch = math.radians(pitch_deg)
            
            # Create and publish joint state message
            joint_state = JointState()
            joint_state.header.stamp = self.get_clock().now().to_msg()
            joint_state.header.frame_id = 'zt30_base_link'
            
            joint_state.name = ['zt30_stand_joint', 'zt30_camera_joint']
            joint_state.position = [-self.current_yaw+math.pi/2, self.current_pitch+math.pi]
            joint_state.velocity = [0.0, 0.0]  # Velocity not available from SIYI SDK
            joint_state.effort = [0.0, 0.0]   # Effort not available from SIYI SDK
            
            self.joint_state_pub.publish(joint_state)
            
        except Exception as e:
            self.get_logger().warn(f"Failed to get gimbal attitude: {e}")
        
    def camera_mode_callback(self, request, response):
        '''
        0x00	Main: Wide, Sub: Zoom (Default)
        0x01	Main: Zoom, Sub: Wide
        0x02	Main: Wide only
        0x03	Main: Zoom only
        0x04	PIP (Wide Main, Zoom Sub)
        0x05	PIP (Zoom Main, Wide Sub)
        0x06	Dual-split (Wide & Zoom)
        0x07	Main: Thermal, Sub: Zoom
        0x08	Main: Thermal, Sub: Wide
        0x09	Thermal only
        0x0A	Dual-split (Thermal + Visible)
        0x0B	PIP (Thermal Main, Zoom Sub)
        '''
        if request.camera_mode == 0:
            mode = 0x00
        elif request.camera_mode == 1:
            mode = 0x01
        elif request.camera_mode == 2:
            mode = 0x02
        elif request.camera_mode == 3:
            mode = 0x03
        elif request.camera_mode == 4:
            mode = 0x04
        elif request.camera_mode == 5:
            mode = 0x05
        elif request.camera_mode == 6:
            mode = 0x06
        elif request.camera_mode == 7:
            mode = 0x07
        elif request.camera_mode == 8:
            mode = 0x08
        elif request.camera_mode == 9:
            mode = 0x09
        elif request.camera_mode == 10:
            mode = 0x0A
        elif request.camera_mode == 11:
            mode = 0x0B
        else:
            self.get_logger().error(self.colorize("Invalid mode", "red"))
            response.success = False
            response.message = "Invalid mode"
            return response
        
        self.cam.setImageMode(mode)  # Thermal as main, Zoom as sub
        self.get_logger().info(self.colorize("Image mode set: Thermal as Main", "cyan"))
        response.success = True
        response.message = "Image mode set to thermal"
        return response
        
    def photo_callback(self, request, response):
        self.cam.requestPhoto()
        self.get_logger().info(self.colorize("Photo triggered", "cyan"))
        response.success = True
        response.message = "Photo triggered"
        return response
    
    def video_callback(self, request, response):
        if self.video_flag:
            self.video_flag = False
            self.get_logger().info(self.colorize("Saving Video Recording", "cyan"))
            self.cam.requestRecording()
            response.success = True
            response.message = "Saving Video Recording"
        else:
            self.video_flag = True
            self.cam.requestRecording()
            self.get_logger().info(self.colorize("Video Recording Started", "cyan"))
            response.success = True
            response.message = "Video Recording Started"
        
        return response

    def camera_gimbal_callback(self, msg):
        self.last_gimbal_msg_time = self.get_clock().now()
        self.gimbal_msg_received = True

        # Handle gimbal movement
        self.cam.requestGimbalSpeed(msg.yaw, msg.pitch)

        # Handle gimbal mode changes
        if msg.gimbal_mode == 1:  # FPV mode
            self.cam.requestFPVMode()
            self.get_logger().info(self.colorize("Switched to FPV mode", "cyan"))
        elif msg.gimbal_mode == 2:  # Lock mode
            self.cam.requestLockMode()
            self.get_logger().info(self.colorize("Switched to Lock mode", "cyan"))
        elif msg.gimbal_mode == 3:  # Follow mode
            self.cam.requestFollowMode()
            self.get_logger().info(self.colorize("Switched to Follow mode", "cyan"))

    def zoomin_control_callback(self, request, response):

        self.cam.requestCurrentZoomLevel()
        curr_zm = self.cam.getCurrentZoomLevel()
        self.get_logger().info(self.colorize(
            f"Zooming in - Current zoom level {curr_zm}", "cyan"))
        self.cam.requestZoomIn()
        response.success = True
        response.message = "Zooming in - Current zoom level: " + str(curr_zm)
        return response

    def zoomout_control_callback(self, request, response):

        self.cam.requestCurrentZoomLevel()
        curr_zm = self.cam.getCurrentZoomLevel()
        self.get_logger().info(self.colorize(
            f"Zooming out - Current zoom level {curr_zm}", "cyan"))
        self.cam.requestZoomOut()
        response.success = True
        response.message = "Zooming out - Current zoom level: " + str(curr_zm)
        return response

    def check_timeout(self):

        now = self.get_clock().now()

        # Check gimbal timeout
        if self.last_gimbal_msg_time:
            elapsed = (now - self.last_gimbal_msg_time).nanoseconds / 1e9
            if elapsed > 1.0 and self.gimbal_msg_received:
                self.get_logger().info(
                    self.colorize("No gimbal message received for 1 second - stopping gimbal", "red"))
                self.cam.requestGimbalSpeed(0, 0)
                self.gimbal_msg_received = False

    def colorize(self, text, color):
        color_codes = {
            'green': '\033[92m',
            'purple': '\033[95m',
            'cyan': '\033[96m',
            'yellow': '\033[93m',
            'orange': '\033[38;5;208m',
            'blue': '\033[94m',
            'red': '\033[91m'
        }
        return color_codes[color] + text + '\033[0m'

    def destroy_node(self):
        self.get_logger().info(self.colorize("Shutting down ROS2 Camera Server", "red"))
        try:
            self.cam.requestGimbalSpeed(0, 0)
            if self.video_flag:
                self.cam.requestRecording()
            self.cam.disconnect()
        except Exception as e:
            self.get_logger().error(f"Error during shutdown: {e}")
        super().destroy_node()