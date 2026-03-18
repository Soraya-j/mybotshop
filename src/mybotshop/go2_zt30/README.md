# ZT30 Control

- Gimbal velocity control
 
```bash
ros2 topic pub /$GO2_NS/zt30/gimbal/control/cmd go2_interface/msg/CameraGimbalCmd '{yaw: 10, pitch: 0}'
```

- Select Camera Mode
    0. Main: Wide, Sub: Zoom (Default)
    1. HD Wide | Thermal Zoom
    2. HD Zoom | HD Wide
    3. HD Wide
    4. HD Wide | PIP (Wide Main, Zoom Sub)
    5. HD Wide | PIP (Zoom Main, Wide Sub)
    6. HD Wide | Dual-split (Wide & Zoom)
    7. Thermal Zoom | Thermal, Sub: Zoom
    8. Main: Thermal, Sub: Wide
    9. Thermal only
    10. Dual-split (Thermal + Visible)
    11. PIP (Thermal Main, Zoom Sub)

- Normal Camera

```bash
ros2 service call /$GO2_NS/zt30/camera_mode go2_interface/srv/CameraMode '{camera_mode: 0}'
```

- Thermal

```bash
ros2 service call /$GO2_NS/zt30/camera_mode go2_interface/srv/CameraMode '{camera_mode: 8}'
```

- Take photo

```bash
ros2 service call /$GO2_NS/zt30/trigger_photo std_srvs/srv/Trigger {}
```

- Start Recording

```bash
ros2 service call /$GO2_NS/zt30/trigger_video std_srvs/srv/Trigger {}
```

- Zoom in

```bash
ros2 service call /$GO2_NS/zt30/zoomin_control std_srvs/srv/Trigger {}
```

- Zoom Out

```bash
ros2 service call /$GO2_NS/zt30/zoomout_control std_srvs/srv/Trigger {}
```