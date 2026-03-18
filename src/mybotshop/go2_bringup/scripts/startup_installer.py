#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Software License Agreement (BSD)
#
# @author    Salman Omar Sohail <support@mybotshop.de>
# @copyright (c) 2025, MYBOTSHOP GmbH, Inc., All rights reserved.

import os
import robot_upstart
from ament_index_python.packages import get_package_share_directory


def install_job(job_name, package_name, launch_filename, domain_id=10,
                e_user=None, rmw_type='rmw_fastrtps_cpp', disable_srvs=False):

    # 0. Print Configuration
    print(color_string(34, "Installing job: {}".format(job_name)))
    print(color_string(34, "Package: {}".format(package_name)))
    print(color_string(34, "Launch File: {}".format(launch_filename)))
    print(color_string(34, "Domain ID: {}".format(domain_id)))
    print(color_string(34, "RMW Type: {}".format(rmw_type)))
    print(color_string(34, "User: {}".format(e_user)))
    print(color_string(34, "Disable Services: {}".format(disable_srvs)))
    print(color_string(34, "----------------------------------------"))

    # 1. Uninstall existing service
    print(color_string(33, "Uninstalling existing service: {}".format(job_name)))
    os.system("sudo service {} stop".format(job_name))
    uninstall_job = robot_upstart.Job(
        name=job_name, rosdistro=os.environ['ROS_DISTRO'])
    uninstall_job.uninstall()

    # 2. Configure new service
    print(color_string(32, "Installing new service: {}".format(job_name)))
    linux_service = robot_upstart.Job(name=job_name,
                                      user=e_user,
                                      ros_domain_id=domain_id,
                                      rmw=rmw_type,
                                      workspace_setup=os.path.join(get_package_share_directory(
                                          'go2_bringup'), 'config/setup.bash')
                                      )

    linux_service.add(package=package_name, filename=launch_filename)
    linux_service.install()

    # 3. Set service state
    if disable_srvs:
        os.system("sudo systemctl disable {}".format(job_name))
        return

    # 4. Refresh for activation
    os.system(
        "sudo systemctl daemon-reload && sudo systemctl start {}".format(job_name))


def color_string(color, string):
    return "\033[{}m{}\033[0m".format(color, string)


if __name__ == "__main__":

    jobs = [
        {"name": "go2-webserver",
         "package": "go2_webserver",
         "launch_filename": "launch/webserver.launch.py",
         },

        {"name": "go2-hardware-controller",
         "package": "go2_platform",
         "launch_filename": "launch/hardware.launch.py",
         "user": "unitree",
         "disable": True},

        {"name": "go2-inbuilt-camera",
         "package": "go2_platform",
         "launch_filename": "launch/camera.launch.py",
         "disable": True},

        {"name": "go2-domain-bridge",
         "package": "go2_platform",
         "launch_filename": "launch/bridge.launch.py",
         "user": "unitree",
         "disable": True},

        {"name": "go2-sensor-fusion",
         "package": "go2_control",
         "launch_filename": "launch/ekf_localization.launch.py",
         "disable": True},

        {"name": "go2-laserscan",
         "package": "go2_platform",
         "launch_filename": "launch/laserscan.launch.py",
         "disable": True},

        {"name": "go2-state-publisher",
         "package": "go2_platform",
         "launch_filename": "launch/state_publisher.launch.py",
         "disable": True},

        {"name": "go2-teleop",
         "package": "go2_control",
         "launch_filename": "launch/teleop.launch.py",
         "disable": True},

        {"name": "go2-description",
         "package": "go2_description",
         "launch_filename": "launch/go2_description.launch.py",
         "disable": True},

        {"name": "go2-realsense-d435i",
         "package": "go2_depth_camera",
         "launch_filename": "launch/realsense_d435i.launch.py",
         "disable": True},
        
        {"name": "go2-hesai-xt16",
         "package": "go2_lidars",
         "launch_filename": "launch/hesai.launch.py",
         "user": "root",
         "disable": True},
        
        # {"name": "go2-obstacle-stopper",
        #  "package": "go2_lidars",
        #  "launch_filename": "launch/obstacle_avoidance.launch.py",
        #  "user": "unitree",
        #  "disable": True},
    ]

    for job in jobs:
        install_job(job_name=job["name"],
                    package_name=job["package"],
                    launch_filename=job["launch_filename"],
                    disable_srvs=job.get("disable", False),
                    rmw_type=job.get("rmw_type", "rmw_cyclonedds_cpp"),
                    e_user=job.get("user", None),
                    )
