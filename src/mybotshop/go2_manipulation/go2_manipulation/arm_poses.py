#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Software License Agreement (BSD)
#
# @author    Salman Omar Sohail <support@mybotshop.de>
# @copyright (c) 2025, MYBOTSHOP GmbH, Inc., All rights reserved.

from builtin_interfaces.msg import Duration
from control_msgs.action import FollowJointTrajectory
from trajectory_msgs.msg import JointTrajectoryPoint

def arm_home(node):
    if not node.action_client_arm.wait_for_server(timeout_sec=20.0):
        print('Action server not available')
        return

    goal_msg = FollowJointTrajectory.Goal()
    goal_msg.trajectory.joint_names = [
        'joint1', 'joint2', 'joint3', 'joint4']
    goal_msg.trajectory.points = [
        JointTrajectoryPoint(
            positions=[0.0, -1.7, 1.4, 1.2], time_from_start=Duration(sec=1)),
    ]
    future = node.action_client_arm.send_goal_async(goal_msg)

def arm_transport(node):
    if not node.action_client_arm.wait_for_server(timeout_sec=20.0):
        print('Action server not available')
        return

    goal_msg = FollowJointTrajectory.Goal()
    goal_msg.trajectory.joint_names = [
        'joint1', 'joint2', 'joint3', 'joint4']
    goal_msg.trajectory.points = [
        JointTrajectoryPoint(
            positions=[0.0, -1.169, 1.378, -0.261], time_from_start=Duration(sec=1)),
    ]
    future = node.action_client_arm.send_goal_async(goal_msg)
    
def arm_stand(node):
    if not node.action_client_arm.wait_for_server(timeout_sec=20.0):
        print('Action server not available')
        return

    goal_msg = FollowJointTrajectory.Goal()
    goal_msg.trajectory.joint_names = [
        'joint1', 'joint2', 'joint3', 'joint4']
    goal_msg.trajectory.points = [
        JointTrajectoryPoint(
            positions=[0.0, 0.0, -0.3, -0.1], time_from_start=Duration(sec=1)),
    ]
    future = node.action_client_arm.send_goal_async(goal_msg)
    
def arm_activate(node):
    if not node.action_client_arm.wait_for_server(timeout_sec=20.0):
        print('Action server not available')
        return

    goal_msg = FollowJointTrajectory.Goal()
    goal_msg.trajectory.joint_names = [
        'joint1', 'joint2', 'joint3', 'joint4']
    goal_msg.trajectory.points = [
        JointTrajectoryPoint(
            positions=[0.0, -1.4, 1.2, 0.0], time_from_start=Duration(sec=1)),
    ]
    future = node.action_client_arm.send_goal_async(goal_msg)

def gripper_close(node):
        if not node.action_client_gripper.wait_for_server(timeout_sec=20.0):
            print('Action server not available')
            return

        goal_msg = FollowJointTrajectory.Goal()
        goal_msg.trajectory.joint_names = ['gripper']
        goal_msg.trajectory.points = [JointTrajectoryPoint(
            positions=[1.0], time_from_start=Duration(sec=0, nanosec=100000000))]
        future = node.action_client_gripper.send_goal_async(goal_msg)
        
def gripper_open(node):
        if not node.action_client_gripper.wait_for_server(timeout_sec=20.0):
            print('Action server not available')
            return

        goal_msg = FollowJointTrajectory.Goal()
        goal_msg.trajectory.joint_names = ['gripper']
        goal_msg.trajectory.points = [JointTrajectoryPoint(
            positions=[-1.0], time_from_start=Duration(sec=0, nanosec=100000000))]
        future = node.action_client_gripper.send_goal_async(goal_msg)