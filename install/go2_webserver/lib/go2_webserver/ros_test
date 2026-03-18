#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Software License Agreement (BSD)
#
# @author    Salman Omar Sohail <support@mybotshop.de>
# @copyright (c) 2025, MYBOTSHOP GmbH, Inc., All rights reserved.
#
# Test script for go2_webserver - publishes mock sensor data
# Run with: ros2 launch go2_webserver ros_test.launch.py
#
# Map Loading:
#   Maps are loaded from test/maps/ folder. The map consists of:
#   - YAML file: Contains metadata (resolution, origin, thresholds)
#   - PGM file: Contains the occupancy grid image data
#
# TF Setup:
#   map -> odom (static transform, identity)
#   odom -> base_link (dynamic transform, follows robot position)

import os
import math
import random
import yaml
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, QoSDurabilityPolicy, QoSReliabilityPolicy
from ament_index_python.packages import get_package_share_directory

import tf2_ros
from nav_msgs.msg import Odometry, OccupancyGrid
from sensor_msgs.msg import NavSatFix, BatteryState, JointState
from diagnostic_msgs.msg import DiagnosticArray, DiagnosticStatus, KeyValue
from geometry_msgs.msg import Quaternion, TransformStamped
from std_msgs.msg import Header


class WebserverTestPublisher(Node):
    """
    Test node that publishes mock sensor data for webserver testing.
    Reads topics from the same YAML config as the webserver.
    """

    def __init__(self):
        super().__init__('webserver_test_publisher')

        self.get_logger().info('\033[92m' + '=' * 60 + '\033[0m')
        self.get_logger().info('\033[92m  Webserver Test Publisher Starting...\033[0m')
        self.get_logger().info('\033[92m' + '=' * 60 + '\033[0m')

        # Declare parameters - same names as webserver for shared YAML config
        # Default map path points to test/maps/test_map.yaml in the package
        default_map_yaml = os.path.join(
            get_package_share_directory('go2_webserver'),
            'test', 'maps', 'test_map.yaml'
        )

        self.declare_parameters(namespace='', parameters=[
            ('robot_odom_topic', 'platform/odom'),
            ('robot_battery_topic', 'platform/battery_state'),
            ('robot_gps_topic', 'fixposition/navsatfix'),
            ('robot_joint_states_topic', 'joint_states'),
            ('robot_diagnostics_topic', '/diagnostics'),
            ('robot_map_topic', 'map'),
            ('robot_gps_lat', 50.95359),
            ('robot_gps_lon', 6.60174),
            ('publish_rate', 10.0),
            ('map_yaml_file', default_map_yaml),
        ])

        # Get parameters
        self.odom_topic = self.get_parameter('robot_odom_topic').value
        self.battery_topic = self.get_parameter('robot_battery_topic').value
        self.gps_topic = self.get_parameter('robot_gps_topic').value
        self.joint_states_topic = self.get_parameter('robot_joint_states_topic').value
        self.diagnostics_topic = self.get_parameter('robot_diagnostics_topic').value
        self.map_topic = self.get_parameter('robot_map_topic').value
        self.gps_lat_origin = self.get_parameter('robot_gps_lat').value
        self.gps_lon_origin = self.get_parameter('robot_gps_lon').value
        self.publish_rate = self.get_parameter('publish_rate').value
        self.map_yaml_file = self.get_parameter('map_yaml_file').value

        # QoS profiles
        qos_transient = QoSProfile(
            depth=10,
            durability=QoSDurabilityPolicy.TRANSIENT_LOCAL
        )
        qos_best_effort = QoSProfile(
            depth=10,
            reliability=QoSReliabilityPolicy.BEST_EFFORT,
            durability=QoSDurabilityPolicy.VOLATILE
        )

        # Publishers
        self.odom_pub = self.create_publisher(Odometry, self.odom_topic, qos_best_effort)
        self.battery_pub = self.create_publisher(BatteryState, self.battery_topic, qos_best_effort)
        self.gps_pub = self.create_publisher(NavSatFix, self.gps_topic, qos_best_effort)
        self.joint_states_pub = self.create_publisher(JointState, self.joint_states_topic, qos_best_effort)
        self.diagnostics_pub = self.create_publisher(DiagnosticArray, self.diagnostics_topic, qos_best_effort)
        self.map_pub = self.create_publisher(OccupancyGrid, self.map_topic, qos_transient)

        # Joint state simulation
        self.joint_names = ['wheel_left', 'wheel_right', 'arm_joint_1', 'arm_joint_2', 'gripper']
        self.joint_positions = [0.0] * len(self.joint_names)
        self.joint_velocities = [0.0] * len(self.joint_names)

        # TF Broadcaster
        self.tf_broadcaster = tf2_ros.TransformBroadcaster(self)
        self.tf_static_broadcaster = tf2_ros.StaticTransformBroadcaster(self)

        # State variables for simulation
        self.time_start = self.get_clock().now()
        self.robot_x = 0.0
        self.robot_y = 0.0
        self.robot_yaw = 0.0
        self.battery_pct = 100.0
        self.gps_lat = self.gps_lat_origin
        self.gps_lon = self.gps_lon_origin

        # Velocity state for varying speeds
        self.linear_speed = 0.0
        self.angular_speed = 0.0
        self.target_linear = 0.5
        self.target_angular = 0.1
        self.velocity_phase = 0.0  # Phase for sine wave variation

        # Publish initial map and static TF
        self.publish_map()
        self.publish_static_tf()

        # Create timers
        period = 1.0 / self.publish_rate
        self.odom_timer = self.create_timer(period, self.publish_odom)
        self.tf_timer = self.create_timer(period, self.publish_tf)
        self.battery_timer = self.create_timer(1.0, self.publish_battery)
        self.gps_timer = self.create_timer(1.0, self.publish_gps)
        self.joint_states_timer = self.create_timer(0.1, self.publish_joint_states)  # 10 Hz
        self.diagnostics_timer = self.create_timer(1.0, self.publish_diagnostics)  # 1 Hz
        self.velocity_timer = self.create_timer(2.0, self.vary_velocity)

        self.get_logger().info(f'\033[94mPublishing to:\033[0m')
        self.get_logger().info(f'  - Odom:        {self.odom_topic} @ {self.publish_rate} Hz')
        self.get_logger().info(f'  - TF:          map->odom->base_link @ {self.publish_rate} Hz')
        self.get_logger().info(f'  - Battery:     {self.battery_topic} @ 1 Hz')
        self.get_logger().info(f'  - GPS:         {self.gps_topic} @ 1 Hz')
        self.get_logger().info(f'  - JointStates: {self.joint_states_topic} @ 10 Hz')
        self.get_logger().info(f'  - Diagnostics: {self.diagnostics_topic} @ 1 Hz')
        self.get_logger().info(f'  - Map:         {self.map_topic} (once)')
        self.get_logger().info(f'\033[94mMap file:\033[0m {self.map_yaml_file}')
        self.get_logger().info(f'\033[94mGPS Origin:\033[0m {self.gps_lat_origin}, {self.gps_lon_origin}')
        self.get_logger().info('\033[93mPress Ctrl+C to stop\033[0m')

    def vary_velocity(self):
        """Vary the velocity over time to simulate acceleration/deceleration."""
        # Randomly change target velocities
        self.target_linear = random.uniform(0.0, 1.0)
        self.target_angular = random.uniform(-0.3, 0.3)

    def publish_odom(self):
        """Publish odometry with simulated motion and continuously varying velocity."""
        msg = Odometry()
        msg.header = Header()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = 'odom'
        msg.child_frame_id = 'base_link'

        # Increment phase for continuous variation
        dt = 1.0 / self.publish_rate
        self.velocity_phase += dt

        # Base velocity from sine waves at different frequencies for organic motion
        # Linear velocity: 0.2 to 0.8 m/s with multiple harmonics
        base_linear = 0.5 + 0.2 * math.sin(self.velocity_phase * 0.5)  # Slow wave
        base_linear += 0.1 * math.sin(self.velocity_phase * 1.3)  # Medium wave
        base_linear += 0.05 * math.sin(self.velocity_phase * 3.7)  # Fast wave

        # Angular velocity: -0.3 to 0.3 rad/s with different frequencies
        base_angular = 0.15 * math.sin(self.velocity_phase * 0.7)  # Slow wave
        base_angular += 0.1 * math.sin(self.velocity_phase * 1.9)  # Medium wave
        base_angular += 0.05 * math.sin(self.velocity_phase * 4.1)  # Fast wave

        # Add random noise for realism
        noise_linear = random.uniform(-0.05, 0.05)
        noise_angular = random.uniform(-0.02, 0.02)

        # Smoothly interpolate towards new values
        accel = 0.15
        target_linear = max(0.0, min(1.0, base_linear + noise_linear))
        target_angular = max(-0.4, min(0.4, base_angular + noise_angular))

        if self.linear_speed < target_linear:
            self.linear_speed = min(self.linear_speed + accel, target_linear)
        else:
            self.linear_speed = max(self.linear_speed - accel, target_linear)

        if self.angular_speed < target_angular:
            self.angular_speed = min(self.angular_speed + accel, target_angular)
        else:
            self.angular_speed = max(self.angular_speed - accel, target_angular)

        # Update position based on velocity
        self.robot_yaw += self.angular_speed * dt
        self.robot_x += self.linear_speed * math.cos(self.robot_yaw) * dt
        self.robot_y += self.linear_speed * math.sin(self.robot_yaw) * dt

        # Keep robot within bounds
        self.robot_x = max(-4.0, min(4.0, self.robot_x))
        self.robot_y = max(-4.0, min(4.0, self.robot_y))

        # Position
        msg.pose.pose.position.x = self.robot_x
        msg.pose.pose.position.y = self.robot_y
        msg.pose.pose.position.z = 0.0

        # Orientation
        msg.pose.pose.orientation = self.yaw_to_quaternion(self.robot_yaw)

        # Velocity
        msg.twist.twist.linear.x = self.linear_speed
        msg.twist.twist.angular.z = self.angular_speed

        self.odom_pub.publish(msg)

    def publish_static_tf(self):
        """Publish static transform from map to odom (identity for testing)."""
        t = TransformStamped()
        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = 'map'
        t.child_frame_id = 'odom'
        t.transform.translation.x = 0.0
        t.transform.translation.y = 0.0
        t.transform.translation.z = 0.0
        t.transform.rotation.x = 0.0
        t.transform.rotation.y = 0.0
        t.transform.rotation.z = 0.0
        t.transform.rotation.w = 1.0
        self.tf_static_broadcaster.sendTransform(t)
        self.get_logger().info('\033[92mStatic TF published: map -> odom\033[0m')

    def publish_tf(self):
        """Publish transform from odom to base_link."""
        t = TransformStamped()
        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = 'odom'
        t.child_frame_id = 'base_link'
        t.transform.translation.x = self.robot_x
        t.transform.translation.y = self.robot_y
        t.transform.translation.z = 0.0
        q = self.yaw_to_quaternion(self.robot_yaw)
        t.transform.rotation.x = q.x
        t.transform.rotation.y = q.y
        t.transform.rotation.z = q.z
        t.transform.rotation.w = q.w
        self.tf_broadcaster.sendTransform(t)

    def publish_battery(self):
        """Publish battery state with slow discharge simulation."""
        msg = BatteryState()
        msg.header = Header()
        msg.header.stamp = self.get_clock().now().to_msg()

        # Simulate slow discharge
        self.battery_pct -= 0.1
        if self.battery_pct < 20.0:
            self.battery_pct = 100.0

        msg.percentage = self.battery_pct / 100.0
        msg.voltage = 24.0 + (self.battery_pct / 100.0) * 4.0
        msg.current = -2.5 + random.uniform(-0.5, 0.5)
        msg.charge = msg.percentage * 20.0
        msg.capacity = 20.0
        msg.design_capacity = 20.0
        msg.power_supply_status = BatteryState.POWER_SUPPLY_STATUS_DISCHARGING
        msg.power_supply_health = BatteryState.POWER_SUPPLY_HEALTH_GOOD
        msg.power_supply_technology = BatteryState.POWER_SUPPLY_TECHNOLOGY_LION
        msg.present = True

        self.battery_pub.publish(msg)

    def publish_gps(self):
        """Publish GPS with small random drift."""
        msg = NavSatFix()
        msg.header = Header()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = 'gps'

        # Add small random drift
        drift_lat = random.uniform(-0.00001, 0.00001)
        drift_lon = random.uniform(-0.00001, 0.00001)

        msg.latitude = self.gps_lat_origin + drift_lat
        msg.longitude = self.gps_lon_origin + drift_lon
        msg.altitude = 50.0 + random.uniform(-1, 1)

        msg.status.status = 0
        msg.status.service = 1

        msg.position_covariance = [1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 2.0]
        msg.position_covariance_type = NavSatFix.COVARIANCE_TYPE_DIAGONAL_KNOWN

        self.gps_pub.publish(msg)

    def publish_joint_states(self):
        """Publish joint states with simulated motion."""
        msg = JointState()
        msg.header = Header()
        msg.header.stamp = self.get_clock().now().to_msg()

        msg.name = self.joint_names

        # Simulate wheel rotation based on robot velocity
        dt = 0.1  # 10 Hz
        wheel_radius = 0.1
        wheel_angular_velocity = self.linear_speed / wheel_radius

        # Update joint positions
        self.joint_positions[0] += wheel_angular_velocity * dt  # left wheel
        self.joint_positions[1] += wheel_angular_velocity * dt  # right wheel
        self.joint_positions[2] = 0.5 * math.sin(self.velocity_phase * 0.3)  # arm oscillation
        self.joint_positions[3] = 0.3 * math.sin(self.velocity_phase * 0.5 + 0.5)
        self.joint_positions[4] = 0.1 * (1 + math.sin(self.velocity_phase * 0.2))  # gripper

        # Velocities
        self.joint_velocities[0] = wheel_angular_velocity
        self.joint_velocities[1] = wheel_angular_velocity
        self.joint_velocities[2] = 0.15 * math.cos(self.velocity_phase * 0.3)
        self.joint_velocities[3] = 0.15 * math.cos(self.velocity_phase * 0.5 + 0.5)
        self.joint_velocities[4] = 0.02 * math.cos(self.velocity_phase * 0.2)

        msg.position = self.joint_positions.copy()
        msg.velocity = self.joint_velocities.copy()
        msg.effort = [random.uniform(0.5, 2.0) for _ in self.joint_names]  # Random effort values

        self.joint_states_pub.publish(msg)

    def publish_diagnostics(self):
        """Publish diagnostic messages for various components."""
        msg = DiagnosticArray()
        msg.header = Header()
        msg.header.stamp = self.get_clock().now().to_msg()

        # Battery diagnostics
        battery_status = DiagnosticStatus()
        battery_status.name = "Battery"
        battery_status.hardware_id = "power_system"
        battery_status.level = DiagnosticStatus.OK if self.battery_pct > 20 else DiagnosticStatus.WARN
        battery_status.message = "Battery OK" if self.battery_pct > 20 else "Low battery"
        battery_status.values = [
            KeyValue(key="Percentage", value=f"{self.battery_pct:.1f}%"),
            KeyValue(key="Voltage", value=f"{24.0 + self.battery_pct / 25:.2f}V"),
            KeyValue(key="Current", value=f"{-2.5 + random.uniform(-0.5, 0.5):.2f}A"),
        ]
        msg.status.append(battery_status)

        # Motor controller diagnostics
        motor_status = DiagnosticStatus()
        motor_status.name = "Motor Controller"
        motor_status.hardware_id = "motors"
        motor_status.level = DiagnosticStatus.OK
        motor_status.message = "All motors operational"
        motor_status.values = [
            KeyValue(key="Left Motor Temp", value=f"{45 + random.uniform(-5, 10):.1f}C"),
            KeyValue(key="Right Motor Temp", value=f"{46 + random.uniform(-5, 10):.1f}C"),
            KeyValue(key="Current Draw", value=f"{abs(self.linear_speed) * 5 + random.uniform(0, 1):.2f}A"),
        ]
        msg.status.append(motor_status)

        # IMU diagnostics
        imu_status = DiagnosticStatus()
        imu_status.name = "IMU Sensor"
        imu_status.hardware_id = "sensors"
        imu_status.level = DiagnosticStatus.OK
        imu_status.message = "IMU readings normal"
        imu_status.values = [
            KeyValue(key="Update Rate", value="100 Hz"),
            KeyValue(key="Temperature", value=f"{35 + random.uniform(-2, 5):.1f}C"),
        ]
        msg.status.append(imu_status)

        # GPS diagnostics
        gps_status = DiagnosticStatus()
        gps_status.name = "GPS Receiver"
        gps_status.hardware_id = "sensors"
        fix_quality = random.choice([DiagnosticStatus.OK, DiagnosticStatus.OK, DiagnosticStatus.WARN])
        gps_status.level = fix_quality
        gps_status.message = "Fix acquired" if fix_quality == DiagnosticStatus.OK else "Weak signal"
        gps_status.values = [
            KeyValue(key="Satellites", value=str(random.randint(6, 12))),
            KeyValue(key="HDOP", value=f"{random.uniform(0.8, 2.5):.2f}"),
            KeyValue(key="Fix Type", value="3D Fix"),
        ]
        msg.status.append(gps_status)

        # Network diagnostics
        network_status = DiagnosticStatus()
        network_status.name = "Network Interface"
        network_status.hardware_id = "network"
        network_status.level = DiagnosticStatus.OK
        network_status.message = "Connected"
        network_status.values = [
            KeyValue(key="IP", value="192.168.1.100"),
            KeyValue(key="Latency", value=f"{random.uniform(1, 10):.1f}ms"),
            KeyValue(key="Bandwidth", value=f"{random.uniform(80, 100):.1f} Mbps"),
        ]
        msg.status.append(network_status)

        self.diagnostics_pub.publish(msg)

    def load_pgm(self, pgm_path):
        """
        Load a PGM (Portable Gray Map) file and return width, height, max_val, and pixel data.
        Supports both ASCII (P2) and binary (P5) PGM formats.
        """
        with open(pgm_path, 'rb') as f:
            # Read magic number
            magic = f.readline().decode('ascii').strip()
            if magic not in ['P2', 'P5']:
                raise ValueError(f"Unsupported PGM format: {magic}")

            # Skip comments
            line = f.readline()
            while line.startswith(b'#'):
                line = f.readline()

            # Read dimensions
            dims = line.decode('ascii').split()
            width, height = int(dims[0]), int(dims[1])

            # Read max value
            max_val = int(f.readline().decode('ascii').strip())

            # Read pixel data
            if magic == 'P5':  # Binary format
                data = list(f.read(width * height))
            else:  # P2 - ASCII format
                data = []
                for line in f:
                    data.extend([int(x) for x in line.decode('ascii').split()])

        return width, height, max_val, data

    def publish_map(self):
        """
        Load and publish occupancy grid map from YAML and PGM files.

        Map file format (nav2 compatible):
        - YAML file contains: image path, resolution, origin, thresholds
        - PGM file contains: grayscale occupancy data (0=occupied, 255=free)

        Conversion: PGM grayscale -> OccupancyGrid values
        - 0 (black) -> 100 (occupied)
        - 255 (white) -> 0 (free)
        - 205 (gray, typical unknown) -> -1 (unknown)
        """
        msg = OccupancyGrid()
        msg.header = Header()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = 'map'

        try:
            # Load map metadata from YAML
            with open(self.map_yaml_file, 'r') as f:
                map_yaml = yaml.safe_load(f)

            # Get PGM file path (relative to YAML file)
            yaml_dir = os.path.dirname(self.map_yaml_file)
            pgm_path = os.path.join(yaml_dir, map_yaml['image'])

            # Load PGM data
            width, height, max_val, pgm_data = self.load_pgm(pgm_path)

            # Set map info from YAML
            msg.info.resolution = float(map_yaml.get('resolution', 0.05))
            msg.info.width = width
            msg.info.height = height

            origin = map_yaml.get('origin', [0.0, 0.0, 0.0])
            msg.info.origin.position.x = float(origin[0])
            msg.info.origin.position.y = float(origin[1])
            msg.info.origin.position.z = 0.0

            # Get thresholds
            occupied_thresh = float(map_yaml.get('occupied_thresh', 0.65))
            free_thresh = float(map_yaml.get('free_thresh', 0.25))
            negate = int(map_yaml.get('negate', 0))

            # Convert PGM data to occupancy grid
            # PGM: 0=black(occupied), 255=white(free), ~205=unknown
            # OccupancyGrid: 0=free, 100=occupied, -1=unknown
            occupancy_data = []
            for pixel in pgm_data:
                # Normalize to 0-1 range
                normalized = pixel / max_val if max_val > 0 else 0

                # Apply negate if specified
                if negate:
                    normalized = 1.0 - normalized

                # Convert to occupancy value based on thresholds
                if normalized >= occupied_thresh:
                    occupancy_data.append(0)  # Free space
                elif normalized <= free_thresh:
                    occupancy_data.append(100)  # Occupied
                else:
                    occupancy_data.append(-1)  # Unknown

            msg.data = occupancy_data
            self.map_pub.publish(msg)
            self.get_logger().info(
                f'\033[92mMap loaded from: {self.map_yaml_file}\033[0m')
            self.get_logger().info(
                f'\033[92mMap size: {width}x{height}, resolution: {msg.info.resolution}m\033[0m')

        except FileNotFoundError as e:
            self.get_logger().warn(f'\033[93mMap file not found: {e}\033[0m')
            self.get_logger().warn('\033[93mPublishing fallback procedural map\033[0m')
            self._publish_fallback_map(msg)
        except Exception as e:
            self.get_logger().error(f'\033[91mError loading map: {e}\033[0m')
            self.get_logger().warn('\033[93mPublishing fallback procedural map\033[0m')
            self._publish_fallback_map(msg)

    def _publish_fallback_map(self, msg):
        """Publish a simple procedural map as fallback if file loading fails."""
        msg.info.resolution = 0.05
        msg.info.width = 200
        msg.info.height = 200
        msg.info.origin.position.x = -5.0
        msg.info.origin.position.y = -5.0

        # Create simple map with walls
        data = [-1] * (200 * 200)
        for y in range(20, 180):
            for x in range(20, 180):
                data[y * 200 + x] = 0
        for i in range(200):
            data[20 * 200 + i] = 100
            data[179 * 200 + i] = 100
            data[i * 200 + 20] = 100
            data[i * 200 + 179] = 100

        msg.data = data
        self.map_pub.publish(msg)
        self.get_logger().info('\033[92mFallback map published (200x200)\033[0m')

    def yaw_to_quaternion(self, yaw):
        """Convert yaw angle to quaternion."""
        q = Quaternion()
        q.x = 0.0
        q.y = 0.0
        q.z = math.sin(yaw / 2.0)
        q.w = math.cos(yaw / 2.0)
        return q


def main(args=None):
    rclpy.init(args=args)

    print('\n' + '=' * 60)
    print('  ROVO Webserver Test Publisher')
    print('  Publishing mock sensor data for testing')
    print('=' * 60 + '\n')

    node = WebserverTestPublisher()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        print('\n\033[93mShutting down test publisher...\033[0m')
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
