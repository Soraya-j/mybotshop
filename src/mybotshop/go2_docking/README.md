# Go2 Docking

Simple Naive approach for docking. Docking should be activated when the robot is atleast 2m away
form the docking station and is facing towards it, this can be coupled with map nav2 to get it near to
the docking station. 

## Setup

1. Print the qr code in config
2. Measure size and change in it ``apriltag.launch.py``
3. Assemble and manually test docking stations
4. Camera setup
   - D435i (Current Implementation)
   - Inbuilt camera (To do)

## Launch

- Launch docking server

```bash
ros2 launch go2_docking docking.launch.py 
```


## Activate Docking Manuever

- Call the service

```bash
ros2 service call /go2/dock std_srvs/srv/Trigger {}
```