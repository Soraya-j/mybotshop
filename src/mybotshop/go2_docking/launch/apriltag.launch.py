#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Software License Agreement (BSD)
#
# @author    Salman Omar Sohail <support@mybotshop.de>
# @copyright (c) 2024, MYBOTSHOP GmbH, Inc., All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, is not permitted without the express permission
# of MYBOTSHOP GmbH.
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

from launch_ros.actions import Node
from launch import LaunchDescription
from launch.actions import SetEnvironmentVariable
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode

def generate_launch_description():

    go2_perception_params = PathJoinSubstitution(
        [FindPackageShare('go2_docking'), 'config', 'go2_perception.yaml'])

    node_go2_april_publisher = Node(
        # namespace='place_holder',
        package='go2_perception',
        executable='apriltag_node',
        output='screen',
        name='go2_perception_apriltag',
        parameters=[go2_perception_params],
    )

    apriltag_composable_node = ComposableNode(
        package='apriltag_ros',
        plugin='AprilTagNode',
        name='apriltag',
        remappings=[
            ('/image_rect', '/d435i/color/image_raw'),
            ('/camera_info', '/d435i/color/camera_info'),
            # ('/image_rect',  '/go2/sensor/camera_raw'),
            # ('/camera_info', '/go2/sensor/camera_info'),
            ('/detections', '/apriltag/detections'),
        ],
        parameters=[{'image_transport': 'raw',
                     'family': '36h11',
                     'size': 0.02,
                     'max_hamming': 0,
                     'detector': {
                         'threads': 1,
                         'decimate': 2.0,
                         'blur': 0.0,
                         'refine': 1,
                         'sharpening': 0.25,
                         'debug': 0
                     },
                     'tag': {
                         'ids': [1, 2, 3],
                         'frames': ['tag_1', 'tag_2', 'tag_3'],
                         'sizes': [0.086, 0.04, 0.04]
                     }}],
        extra_arguments=[{'use_intra_process_comms': True}]
    )

    apriltag_container = ComposableNodeContainer(
        name='apriltag_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[apriltag_composable_node],
        output='screen',
    )

    ld = LaunchDescription()
    
    ld.add_action(apriltag_container)
    # ld.add_action(node_go2_april_publisher)

    return ld