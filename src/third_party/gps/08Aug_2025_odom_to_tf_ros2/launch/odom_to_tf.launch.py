#!/usr/bin/env python3
import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import SetEnvironmentVariable

def generate_launch_description():
    # Set ROS domain ID
    odom_to_tf_domain_id = SetEnvironmentVariable('ROS_DOMAIN_ID', '10')

    # Get namespace from environment variable, default if not set
    nsp = os.environ.get('GO2_NS', 'go2_unit_001')

    # Remapping rules
    rmp = [
        ('/tf', 'tf'),
        ('/tf_static', 'tf_static'),
        ('/diagnostics', 'diagnostics'),
    ]

    # Path to config file
    config_file_path = os.path.join(
        get_package_share_directory("odom_to_tf_ros2"),
        "config",
        "odom_to_tf.yaml"
    )

    # Node definition
    odom_to_tf_node = Node(
        namespace=nsp,
        remappings=rmp,
        name="odom_to_tf",
        package="odom_to_tf_ros2",
        executable="odom_to_tf",
        output="screen",
        parameters=[config_file_path],
    )
    
    static_tf_ecef_to_odom = Node(
        namespace=nsp,
        remappings=rmp,
        package="tf2_ros",
        executable="static_transform_publisher",
        arguments=[
            "0", "0", "0",    # translation
            "0", "0", "0",    # rotation  
            "FP_ECEF",        # parent frame (from your odom data)
            "odom"            # child frame (standard ROS)
        ],
        output="screen"
    )
    
    # Launch description
    ld = LaunchDescription()
    ld.add_action(odom_to_tf_domain_id)
    ld.add_action(odom_to_tf_node)
    ld.add_action(static_tf_ecef_to_odom)

    return ld
