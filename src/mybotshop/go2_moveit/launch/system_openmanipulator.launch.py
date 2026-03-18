# Copyright 2020 Yutaka Kondo <yutaka.kondo@youtalk.jp>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import xacro

from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    robot_description_config = xacro.process_file(os.path.join(get_package_share_directory("go2_description"), "xacro", "robot.xacro"))
    controller_config = os.path.join(get_package_share_directory("go2_moveit"), "config", "ros2_controllers.yaml")

    ma_controller_manager =Node(
            namespace="open_manipulator",
            package="controller_manager",
            executable="ros2_control_node",
            parameters=[
                {"robot_description": robot_description_config.toxml()}, controller_config],
            output={
                "stdout": "screen",
                "stderr": "screen",
            },
        )

    js_controller_manager =Node(
        namespace="open_manipulator",
        package="controller_manager",
        executable="spawner.py",
        arguments=["joint_state_broadcaster", "--controller-manager", "/open_manipulator/controller_manager"],
    )

    vl_controller_manager =Node(
        namespace="open_manipulator",
        package="controller_manager",
        executable="spawner.py",
        arguments=["velocity_controller", "-c", "/open_manipulator/controller_manager"],
    )

    jt_controller_manager =Node(
        namespace="open_manipulator",
        package="controller_manager",
        executable="spawner.py",
        arguments=["joint_trajectory_controller", "-c", "/open_manipulator/controller_manager"],
    )

    gr_controller_manager =Node(
        namespace="open_manipulator",
        package="controller_manager",
        executable="spawner.py",
        arguments=["gripper_controller", "-c", "/open_manipulator/controller_manager"],
    )
    
    ld = LaunchDescription()

    ld.add_action(ma_controller_manager)
    ld.add_action(js_controller_manager)
    # ld.add_action(vl_controller_manager)
    ld.add_action(gr_controller_manager)
    ld.add_action(jt_controller_manager)

    return ld