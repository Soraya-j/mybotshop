#! /usr/bin/env python3
# Copyright 2024 Open Navigation LLC

from opennav_docking_msgs.action import DockRobot
import rclpy


def dockRobot(self, dock_id = ""):
    """Send a `DockRobot` action request."""
    print("Waiting for 'DockRobot' action server")
    while not self.docking_client.wait_for_server(timeout_sec=1.0):
        print('"DockRobot" action server not available, waiting...')

    goal_msg = DockRobot.Goal()
    goal_msg.use_dock_id = True
    goal_msg.dock_id = dock_id  # if wanting to use ID instead

    print('Docking at ID: ' + str(dock_id) + '...')
    send_goal_future = self.docking_client.send_goal_async(goal_msg,
                                                            self._feedbackCallback)
    rclpy.spin_until_future_complete(self, send_goal_future)
    self.goal_handle = send_goal_future.result()

    if not self.goal_handle.accepted:
        print('Docking request was rejected!')
        return False

    self.result_future = self.goal_handle.get_result_async()
    return True

...

dock_id = 'home_dock'
dockRobot(dock_id)