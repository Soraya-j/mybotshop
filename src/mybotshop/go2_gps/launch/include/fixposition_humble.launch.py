#!/usr/bin/env python3

# Software License Agreement (BSD)
# Author: Salman Omar Sohail <support@mybotshop.de>
# Copyright (c) 2024, MYBOTSHOP GmbH, Inc., All rights reserved.

import os

from launch import LaunchDescription
from launch.actions import SetEnvironmentVariable
from launch.substitutions import PathJoinSubstitution, LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():

    # Set ROS domain ID
    go2_control_domain_id = SetEnvironmentVariable('ROS_DOMAIN_ID', '10')

    # Namespace
    nsp = os.environ.get('GO2_NS', 'go2_unit_0001')

    # Remappings
    rmp = [
        ('/diagnostics', 'diagnostics'),
        ('/tf', 'tf'),
        ('/tf_static', 'tf_static'),
    ]

    # Parameters
    config_fixposition = PathJoinSubstitution([
        FindPackageShare('go2_gps'),
        'config',
        'fixposition.yaml'
    ])

    # Node definition
    node_fixposition = Node(
        namespace=nsp,
        package='fixposition_driver_ros2',
        executable='fixposition_driver_ros2_exec',
        name='fixposition_driver_ros2',
        output='screen',
        respawn=True,
        respawn_delay=5.0,
        remappings=rmp,
        parameters=[
            config_fixposition,
            {'some_numeric_param': 100.2}
        ],
        arguments=['--ros-args', '--log-level', 'fixposition_driver_ros2:=info'],
        # Optional launch prefix
        prefix=os.environ.get('FIXPOS_LAUNCHER', '')  # If needed
    )

    # Build LaunchDescription
    ld = LaunchDescription()
    ld.add_action(go2_control_domain_id)
    ld.add_action(node_fixposition)

    return ld
