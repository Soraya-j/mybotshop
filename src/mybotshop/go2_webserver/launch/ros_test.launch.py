#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Software License Agreement (BSD)
#
# @author    Salman Omar Sohail <support@mybotshop.de>
# @copyright (c) 2025, MYBOTSHOP GmbH, Inc., All rights reserved.
#
# Launch file for webserver test publisher
# Run with: ros2 launch go2_webserver ros_test.launch.py

import os
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():

    nsp = os.environ.get('GO2_NS', 'go2_unit_001')
    rmp = [
        ('/tf', 'tf'),
        ('/tf_static', 'tf_static'),
    ]

    # Path to config file (same as webserver)
    filepath_config = PathJoinSubstitution(
        [FindPackageShare('go2_webserver'), 'config', 'robot_webserver.yaml']
    )

    # Test publisher node - loads from same YAML as webserver
    node_test_publisher = Node(
        namespace=nsp,
        remappings=rmp,
        name='webserver_test_publisher',
        package='go2_webserver',
        executable='ros_test',
        output='screen',
        emulate_tty=True,
        parameters=[filepath_config]
    )

    # Camera node for video streaming
    node_camera = Node(
        namespace=nsp,
        name='v4l2_camera_node',
        package='v4l2_camera',
        executable='v4l2_camera_node',
        output='screen',
        emulate_tty=True,
    )

    ld = LaunchDescription()
    ld.add_action(node_test_publisher)
    ld.add_action(node_camera)

    return ld
