# MYBOTSHOP Web Interface
![maintainer](https://img.shields.io/badge/Maintainer-Salman-blue)

## Operation

- Add password to configuration file and service names

- Set VNC password

```bash
vncpasswd ~/.vnc/passwd
Password: mybotshop
Verify: mybotshop
Would you like to enter a view-only password (y/n)? y
Password: mybotshop
Verify: mybotshop
```

- Run WebServer

```bash
ros2 launch go2_webserver webserver.launch.py
```

## Dependency

Core dependency

- ROS2
- `pip3 install Flask`

For Audio

- `pip3 install playsound`
- `pip3 install TTS`

For Audio CLI

- `sudo apt-get install espeak-ng`
    
## Configuration

- Update the ``robot_webserver.yaml``
- Update `.gltf` & name in `index.html` and `system.html`
- Update `video stream script`

## Parameters

This file contains the configuration parameters for the go2_webserver node. 
The parameters are used to configure the web server, including the IP address and port, 
as well as the robot's GPS coordinates, password, and other settings.

- `str` robot_rosbag_dir -- Directory where the rosbag files are stored
- `int` robot_rosbag_storage -- Split rosbag is file size in more that 1 Gb (in bytes)
- `int` robot_rosbag_duration -- Split rosbag if duration is more than 30 minutes (in seconds)
- `lis` robot_services -- List of services to be used by the webserver
- `str` robot_webserver -- Name of the webserver node
- `str` robot_map_topic -- Name of the map topic
- `str` robot_cmd_vel -- Name of the cmd_vel topic
- `str` robot_e_stop -- Name of the e_stop topic
- `flo` robot_gps_lat -- Latitude of the robot
- `flo` robot_gps_lon -- Longitude of the robot
- `str` robot_password -- Password for the robot
- `str` robot_gps_topic -- Name of the gps topic
- `str` robot_battery_topic -- Name of the battery topic

# Speed up

```bash
cd src/mybotshop/go2_webserver/go2_webserver
../scripts/compile_binary.sh
```

# Test

## Test Waypoints

```bash
ros2 topic pub -1 /$GO2_NS/fixposition/navsatfix sensor_msgs/msg/NavSatFix "{
  header: {
    stamp: {sec: 0, nanosec: 0},
    frame_id: 'gps'
  },
  status: {
    status: 0,
    service: 1
  },
  latitude: 50.95358,
  longitude: 6.60168,
  altitude: 70.0,
  position_covariance: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
  position_covariance_type: 0
}"
```

## Test Camera

```bash
ros2 run v4l2_camera v4l2_camera_node
```

## Test odom

```bash
ros2 topic pub /$GO2_NS/platform/odom nav_msgs/msg/Odometry "{
  header: {
    stamp: {sec: 0, nanosec: 0},
    frame_id: 'odom'
  },
  child_frame_id: 'base_link',
  pose: {
    pose: {
      position: {x: 1.0, y: 2.0, z: 0.0},
      orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}
    }
  },
  twist: {
    twist: {
      linear:  {x: 0.5, y: 0.0, z: 0.0},
      angular: {x: 0.0, y: 0.0, z: 0.1}
    }
  }
}"
```

## Test Map

```bash
export GO2_NS=rovo_unit_022
```

```bash
ros2 run nav2_map_server map_server --ros-args -p yaml_filename:=/home/administrator/projects/tracked_robots/mbs_rovo3/src/mybotshop/rovo_navigation/maps/custom_map.yaml --ros-args --remap map:=/$GO2_NS/map --ros-args --remap map_server/transition_event:=/$GO2_NS/map_server/transition_event
```

```bash
ros2 lifecycle set /map_server configure && ros2 lifecycle set /map_server activate
```

```bash
ros2 topic pub /$GO2_NS/map nav_msgs/msg/OccupancyGrid '{"info": {"resolution": 1.0, "width": 1, "height": 1, "origin": {"position": {"x": 0.0, "y": 0.0, "z":0.0}, "orientation":{"x":0.0,"y":0.0,"z":0.0,"w":1.0}}}, "data":[0]}'
```

```bash
ros2 run tf2_ros static_transform_publisher 0 0 0 0 0 0 map odom --ros-args --remap tf:=/$GO2_NS/tf --ros-args --remap tf_static:=/$GO2_NS/tf_static
```

```bash
ros2 run tf2_ros static_transform_publisher 0 0 0 0 0 0 odom base_link --ros-args --remap tf:=/$GO2_NS/tf --ros-args --remap tf_static:=/$GO2_NS/tf_static
```

## Test Battery

```bash
# 10%
ros2 topic pub /$GO2_NS/platform/bms/state sensor_msgs/msg/BatteryState \
"{percentage: 0.1}" -1

# 50%
ros2 topic pub /$GO2_NS/platform/battery_state sensor_msgs/msg/BatteryState \
"{percentage: 0.5}" -1

# 100%
ros2 topic pub /$GO2_NS/platform/bms/state sensor_msgs/msg/BatteryState \
"{percentage: 1.0}" -1
```

## Generate ssl certificate

```bash
openssl req -x509 -newkey rsa:4096 -nodes -keyout key.pem -out cert.pem -days 3650 \
-subj "/C=DE/ST=NRW/L=Cologne/O=MYBOTSHOP/OU=Tech/CN=docs.mybotshop.de"
```

## Generate Binaries

```bash
cd /opt/mybotshop/src/mybotshop/go2_webserver/go2_webserver
../scripts/compile_binary.sh
```
```bash
cd /opt/mybotshop/src/mybotshop/go2_webserver/go2_webserver/ && rm -rf librosnode.py libaudiogen.py  libwebserver.py
```