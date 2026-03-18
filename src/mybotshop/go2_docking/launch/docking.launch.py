#!/usr/bin/env python3

# Copyright (c) 2024 Open Navigation LLC
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

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():

    go2_docking_params = PathJoinSubstitution(
        [FindPackageShare('go2_docking'), 'config', 'go2_docking.yaml'])
    
    remappings = [('/tf', 'tf'),
                  ('/tf_static', 'tf_static'),   ]
    
    node_docking_server = Node(
        package='go2_docking',
        executable='dock_navi',
        name='docking_server',
        namespace='go2',
        output='screen',
        # remappings=remappings,
        parameters=[go2_docking_params]
    )
    
    launch_april_tag_publisher = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(PathJoinSubstitution(
        [FindPackageShare("go2_docking"), 'launch', 'apriltag.launch.py'])))
    
    ld = LaunchDescription()
    
    ld.add_action(launch_april_tag_publisher)
    ld.add_action(node_docking_server)

    return ld