#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Software License Agreement (BSD)
#
# @author    Salman Omar Sohail <support@mybotshop.de>
# @copyright (c) 2025, MYBOTSHOP GmbH, Inc., All rights reserved.


from std_msgs.msg import Bool

# -------------------------------------------- Import Custom Interfaces
from go2_interface.srv import Go2Modes, Go2Light, Go2Volume
from go2_webserver.libgpsconverter import GpsConverter

# -------------------------------------------- Advanced Features
from std_srvs.srv import Trigger, SetBool
from std_msgs.msg import Bool, Empty, String, Float64

# -------------------------------------------- Open Source Web Interface
class ROS2CustomWebInterface():

    def __init__(self, web_node):

        self.web_node = web_node

        # -------------------------------------------- Service Clients
        self.go2_robot_service_client = self.web_node.create_client(Go2Modes, 'modes')
        self.go2_light_service_client = self.web_node.create_client(Go2Light, 'light')
        self.go2_volume_service_client = self.web_node.create_client(Go2Volume, 'volume')

        # -------------------------------------------- State Variables
        self.light_level = 0
        self.volume_level = 0

        self.web_node.create_timer(
            1, self.ros2_webserver_control_buttons_callback)

        # -------------------------------------------- Advanced Features
        # self.llm = ROS2LLM(self.web_node)
        self.gps_converter = GpsConverter(self.web_node)

        self.web_node.get_logger().info(self.web_node.colorize(
            "ROS2 Custom Web Interface Loaded", "green"))

    # -------------------------------------------- Go2 Mode Control
    def set_go2_mode(self, mode: str):
        """Set Go2 robot mode via Go2Modes service."""
        if not self.go2_robot_service_client.wait_for_service(timeout_sec=1.0):
            self.web_node.get_logger().warning("Go2 modes service not available")
            return False

        request = Go2Modes.Request()
        request.request_data = mode

        future = self.go2_robot_service_client.call_async(request)
        future.add_done_callback(
            lambda f: self._go2_mode_callback(f, mode))
        return True

    def _go2_mode_callback(self, future, mode: str):
        """Handle Go2 mode service response."""
        try:
            response = future.result()
            self.web_node.get_logger().info(
                self.web_node.colorize(f"Go2 mode '{mode}' executed", "green"))
        except Exception as e:
            self.web_node.get_logger().error(f"Go2 mode service error: {e}")

    # -------------------------------------------- Light Control
    def increase_light(self):
        """Increase light level (0-10)."""
        if self.light_level < 10:
            self.light_level += 1
            self._set_light_level()
            return True
        return False

    def decrease_light(self):
        """Decrease light level (0-10)."""
        if self.light_level > 0:
            self.light_level -= 1
            self._set_light_level()
            return True
        return False

    def _set_light_level(self):
        """Send light level to Go2 robot."""
        if not self.go2_light_service_client.wait_for_service(timeout_sec=1.0):
            self.web_node.get_logger().warning("Go2 light service not available")
            return

        request = Go2Light.Request()
        request.light_level = self.light_level

        future = self.go2_light_service_client.call_async(request)
        future.add_done_callback(
            lambda f: self.web_node.get_logger().info(
                self.web_node.colorize(f"Light level set to {self.light_level}", "green")))

    # -------------------------------------------- Volume Control
    def increase_volume(self):
        """Increase volume level (0-10)."""
        if self.volume_level < 10:
            self.volume_level += 1
            self._set_volume_level()
            return True
        return False

    def decrease_volume(self):
        """Decrease volume level (0-10)."""
        if self.volume_level > 0:
            self.volume_level -= 1
            self._set_volume_level()
            return True
        return False

    def _set_volume_level(self):
        """Send volume level to Go2 robot."""
        if not self.go2_volume_service_client.wait_for_service(timeout_sec=1.0):
            self.web_node.get_logger().warning("Go2 volume service not available")
            return

        request = Go2Volume.Request()
        request.volume_level = self.volume_level

        future = self.go2_volume_service_client.call_async(request)
        future.add_done_callback(
            lambda f: self.web_node.get_logger().info(
                self.web_node.colorize(f"Volume level set to {self.volume_level}", "green")))

    # -------------------------------------------- Webserver Control Buttons
    def ros2_webserver_control_buttons_callback(self):
        # Define control block actions for maintainability
        # Format: block_id -> (description, action_callable or None)
        block_actions = {
            # Block 1: Robot Movement Control
            "11": ("Stand Up", lambda: self.set_go2_mode('stand_up')),
            "12": ("Gait Classic", lambda: self.set_go2_mode('gait_type_classic')),
            "13": ("Gait Economic", lambda: self.set_go2_mode('gait_type_economic')),
            "14": ("Block 14 active", None),
            "15": ("Stand Down", lambda: self.set_go2_mode('stand_down')),
            "16": ("Gait Trot Run", lambda: self.set_go2_mode('gait_type_trot_run')),
            "17": ("Gait Agile", lambda: self.set_go2_mode('gait_type_agile')),
            "18": ("Block 18 active", None),
            # Block 2: Light and Volume Control
            "21": ("Increase Light", lambda: self.increase_light()),
            "22": ("Increase Volume", lambda: self.increase_volume()),
            "23": ("Block 23 active", None),
            "24": ("Block 24 active", None),
            "25": ("Decrease Light", lambda: self.decrease_light()),
            "26": ("Decrease Volume", lambda: self.decrease_volume()),
            "27": ("Block 27 active", None),
            "28": ("Block 28 active", None),
            # Block 3: Reserved
            "31": ("Block 31 active", None),
            "32": ("Block 32 active", None),
            "33": ("Block 33 active", None),
            "34": ("Block 34 active", None),
            "35": ("Block 35 active", None),
            "36": ("Block 36 active", None),
            "37": ("Block 37 active", None),
            "38": ("Block 38 active", None),
            # Block 4: System Commands
            "41": ("Rebooting system", lambda: self.web_node.sudo_command_line("sudo -S shutdown -r now")),
            "42": ("System update", None),  # Disabled: sudo -S apt-get update
            "43": ("Clear journal", None),  # Disabled: journalctl --vacuum-time=7d
            "44": ("Reload udev", lambda: self.web_node.sudo_command_line(
                "sudo -S udevadm control --reload && sudo -S udevadm trigger")),
            "45": ("Shutdown system", lambda: self.web_node.sudo_command_line("sudo -S shutdown -h now")),
            "46": ("System upgrade", None),  # Disabled: sudo -S apt upgrade -y
            "47": ("Restart network", lambda: self.web_node.sudo_command_line(
                "sudo -S systemctl restart NetworkManager")),
            "48": ("Clear memory cache", lambda: self.web_node.sudo_command_line(
                "sudo -S sync; sudo -S sysctl -w vm.drop_caches=3")),
            # LLM
            "gpt": ("GPT active", lambda: self.llm.driver() if hasattr(self, 'llm') else None),
        }

        try:
            # Process control blocks using dictionary lookup
            for block_id, (description, action) in block_actions.items():
                if self.web_node.control_blocks.get(block_id, False):
                    self.web_node.get_logger().info(f"Control Block {block_id}: {description}")
                    if action is not None:
                        action()
                    self.web_node.control_blocks[block_id] = False
                    break  # Only process one block per callback (maintains original elif behavior)

            # -------------------------------------------- GPS Navigation
            if self.web_node.gps_navi:
                self.web_node.get_logger().info("GPS Waypoint is active")
                self.gps_converter.convert_waypoints()
                self.web_node.gps_navi = False

        except Exception as e:
            self.web_node.get_logger().error(
                f"Error in ros2_webserver_control_buttons_callback: {e}")

        # -------------------------------------------- Slider Rig
        try:
            any_update = (
                self.web_node.rig1 is not None or
                self.web_node.rig2 is not None or
                self.web_node.rig3 is not None
            )

            if any_update:
                if self.web_node.rig1 is not None:
                    self.web_node.get_logger().info(f"Rig 1: {self.web_node.rig1}")
                    # TODO: apply rig1 command here
                    self.web_node.rig1 = None

                if self.web_node.rig2 is not None:
                    self.web_node.get_logger().info(f"Rig 2: {self.web_node.rig2}")
                    # TODO: apply rig2 command here
                    self.web_node.rig2 = None

                if self.web_node.rig3 is not None:
                    self.web_node.get_logger().info(f"Rig 3: {self.web_node.rig3}")
                    # TODO: apply rig3 command here
                    self.web_node.rig3 = None
        except Exception as e:
            self.web_node.get_logger().error(f"Rig Slider Error: {e}")

        # -------------------------------------------- Robot E-Stop
        try:
            if self.web_node.e_stop == 200:
                self.web_node.robot_estop.publish(Bool(data=True))
            if self.web_node.e_stop == 250:
                self.web_node.robot_estop.publish(Bool(data=False))
                self.web_node.e_stop = None
        except Exception as e:
            self.web_node.get_logger().error(f"Robot E-Stop Error: {e}")
