#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
from launch_ros.actions import Node
from launch import LaunchDescription
from launch.actions import SetEnvironmentVariable

def generate_launch_description():

    go2_control_domain_id = SetEnvironmentVariable('ROS_DOMAIN_ID', '10')
    
    nsp = os.environ.get('GO2_NS', 'go2_unit_001')
    rmp = [
        ('/tf', 'tf'),
        ('/tf_static', 'tf_static'),
    ]

    node_hesai = Node(
        namespace=nsp,
        remappings=rmp,
        respawn=True,
        package='hesai_ros_driver',
        executable='hesai_ros_driver_node',
        output='screen'
    )

    ld = LaunchDescription()
    
    ld.add_action(go2_control_domain_id)
    ld.add_action(node_hesai)

    return ld
