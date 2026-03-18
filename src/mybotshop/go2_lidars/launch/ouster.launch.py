# Copyright 2023 Ouster, Inc.

"""Launch a sensor node along with os_cloud and os_"""

import os
import launch
import lifecycle_msgs.msg
from pathlib import Path

from ament_index_python.packages import get_package_share_directory
from launch_ros.actions import LifecycleNode
from launch.actions import (DeclareLaunchArgument, SetEnvironmentVariable,
                            RegisterEventHandler, EmitEvent, LogInfo)
from launch.substitutions import LaunchConfiguration
from launch.events import matches_action
from launch_ros.events.lifecycle import ChangeState
from launch_ros.event_handlers import OnStateTransition


def generate_launch_description():
    """
    Generate launch description for running go2_sensors components separately each
    component will run in a separate process).
    """
    go2_control_domain_id = SetEnvironmentVariable('ROS_DOMAIN_ID', '10')

    nsp = os.environ.get('GO2_NS', 'go2_unit_001')
    rmp = [
        ('/tf', 'tf'),
        ('/tf_static', 'tf_static'),
    ]
    
    go2_sensors_pkg_dir = get_package_share_directory('go2_lidars')
    default_params_file = \
        Path(go2_sensors_pkg_dir) / 'config' / 'ouster_params.yaml'
    params_file = LaunchConfiguration('params_file')
    params_file_arg = DeclareLaunchArgument('params_file',
                                            default_value=str(
                                            default_params_file),
                                            description='name or path to the parameters file to use.')

    ouster_ns = LaunchConfiguration('ouster_ns')
    ouster_ns_arg = DeclareLaunchArgument(
        'ouster_ns', default_value=nsp+'/ouster',)

    os_driver_name = LaunchConfiguration('os_driver_name')
    os_driver_name_arg = DeclareLaunchArgument('os_driver_name', default_value='os_driver')

    os_driver = LifecycleNode(
        namespace=ouster_ns,
        remappings=rmp,
        package='ouster_ros',
        executable='os_driver',
        name=os_driver_name,
        parameters=[params_file],
        output='screen',
    )

    sensor_configure_event = EmitEvent(
        event=ChangeState(
            lifecycle_node_matcher=matches_action(os_driver),
            transition_id=lifecycle_msgs.msg.Transition.TRANSITION_CONFIGURE,
        )
    )

    sensor_activate_event = RegisterEventHandler(
        OnStateTransition(
            target_lifecycle_node=os_driver, goal_state='inactive',
            entities=[
                LogInfo(msg="os_driver activating..."),
                EmitEvent(event=ChangeState(
                    lifecycle_node_matcher=matches_action(os_driver),
                    transition_id=lifecycle_msgs.msg.Transition.TRANSITION_ACTIVATE,
                )),
            ],
            handle_once=True
        )
    )

    return launch.LaunchDescription([
        go2_control_domain_id,
        params_file_arg,
        ouster_ns_arg,
        os_driver_name_arg,
        os_driver,
        sensor_configure_event,
        sensor_activate_event,
    ])