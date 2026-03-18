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
import yaml
import xacro

from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from launch.actions import SetEnvironmentVariable

def replace_namespace(data, old_ns, new_ns):
    if isinstance(data, dict):
        return {
            key.replace(old_ns, new_ns): replace_namespace(value, old_ns, new_ns)
            for key, value in data.items()
        }
    elif isinstance(data, list):
        return [replace_namespace(item, old_ns, new_ns) for item in data]
    elif isinstance(data, str):
        return data.replace(old_ns, new_ns)
    else:
        return data

def modify_namespace():
    # Get the namespace from the environment variable
    nsp = os.environ.get('GO2_NS', 'go2_unit_001')

    # Path to the YAML file
    controller_manager_config = os.path.join(
        get_package_share_directory("go2_moveit"),
        "config",
        "ros2_controllers.yaml"
    )

    # Read the YAML file
    with open(controller_manager_config, 'r') as f:
        yaml_data = yaml.safe_load(f)

    # Replace 'required_namespace' with 'nsp'
    modified_yaml_data = replace_namespace(
        yaml_data, 'required_namespace', nsp)

    # Write the modified data back to the YAML file
    with open(controller_manager_config, 'w') as f:
        yaml.dump(modified_yaml_data, f, default_flow_style=False)


modify_namespace()


def generate_launch_description():

    go2_control_domain_id = SetEnvironmentVariable('ROS_DOMAIN_ID', '10')

    nsp = os.environ.get('GO2_NS', 'go2_unit_001')
    
    rmp = [
        ('/tf', 'tf'),
        ('/tf_static', 'tf_static'),
        ('/diagnostics', 'diagnostics'),
    ]

    robot_description_config = xacro.process_file(os.path.join(
        get_package_share_directory("go2_description"), "xacro", "robot.xacro"))

    controller_manager_config = os.path.join(get_package_share_directory(
        "go2_moveit"), "config", "ros2_controllers.yaml")

    ma_controller_manager = Node(
        namespace=nsp+"/open_manipulator",
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[
                {"robot_description": robot_description_config.toxml()}, controller_manager_config],
        output={
            "stdout": "screen",
            "stderr": "screen",
        },
        # prefix=["nice -n -10"],  
    )

    js_controller_manager = Node(
        namespace=nsp+"/open_manipulator",
        package="controller_manager",
        executable="spawner.py",
        arguments=["joint_state_broadcaster", "--controller-manager",
                   "/"+nsp+"/open_manipulator/controller_manager"],
    )

    vl_controller_manager = Node(
        namespace=nsp+"/open_manipulator",
        package="controller_manager",
        executable="spawner.py",
        arguments=["velocity_controller", "-c", "/" +
                   nsp+"/open_manipulator/controller_manager"],
    )

    jt_controller_manager = Node(
        namespace=nsp+"/open_manipulator",
        package="controller_manager",
        executable="spawner.py",
        arguments=["joint_trajectory_controller", "-c",
                   "/"+nsp+"/open_manipulator/controller_manager"],
    )

    gr_controller_manager = Node(
        namespace=nsp+"/open_manipulator",
        package="controller_manager",
        executable="spawner.py",
        arguments=["gripper_controller", "-c", "/" +
                   nsp+"/open_manipulator/controller_manager"],
    )
   
    openmanipulator_bridge_node = Node(
        namespace=nsp,
        remappings=rmp,
        package='go2_manipulation',
        executable='openmanipulator_bridge',
        name='openmanipulator_bridge',
        output='screen',
    )

    ld = LaunchDescription()

    ld.add_action(go2_control_domain_id)

    ld.add_action(ma_controller_manager)
    ld.add_action(js_controller_manager)
    # ld.add_action(vl_controller_manager)
    ld.add_action(gr_controller_manager)
    ld.add_action(jt_controller_manager)
    ld.add_action(openmanipulator_bridge_node)

    return ld