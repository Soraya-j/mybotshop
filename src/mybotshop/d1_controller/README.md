# Enable Motors

```bash
ROS_DOMAIN_ID=10 ros2 service call /$GO2_NS/d1/enable_load std_srvs/srv/SetBool "{data: true}"
```

# Disable Motors

```bash
ROS_DOMAIN_ID=10 ros2 service call /$GO2_NS/d1/enable_load std_srvs/srv/SetBool "{data: false}"
```


# Move to home position (all joints in radians)

```bash
ROS_DOMAIN_ID=10 ros2 action send_goal /$GO2_NS/d1/follow_joint_trajectory control_msgs/action/FollowJointTrajectory "
trajectory:
  joint_names: ['d1_link1_joint', 'd1_link2_joint', 'd1_link3_joint', 'd1_link4_joint', 'd1_link5_joint', 'd1_link6_joint', 'd1_link_l_joint']
  points:
  - positions: [0.0, -1.047, 1.047, 0.0, 0.524, 0.0, 0.0]
    time_from_start: {sec: 3, nanosec: 0}
"
```