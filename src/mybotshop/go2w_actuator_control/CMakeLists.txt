cmake_minimum_required(VERSION 3.10)
project(go2w_actuator_control)

# Find ROS2 Dependencies
find_package(ament_cmake REQUIRED)

# Install Python executable
install(PROGRAMS
  scripts/go2w_stand_sit.py
  DESTINATION lib/${PROJECT_NAME}
)

# Install launch files
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
