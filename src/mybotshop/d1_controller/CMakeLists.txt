cmake_minimum_required(VERSION 3.10)
project(d1_controller)

# Set C++ standard 
SET(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17)

# Build settings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIC")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
endif()

# Find ROS2 dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(control_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(trajectory_msgs REQUIRED)

# Include directories 
include_directories(
  include
  include/ddscxx
  /usr/local/include/iceoryx/v2.0.2
  ${CMAKE_CURRENT_SOURCE_DIR}/src/msg
)

# Link libraries 
link_libraries(unitree_sdk2 ddsc ddscxx rt pthread)

# Set ROS2 dependencies list
set(DEPENDENCY_LIST
  rclcpp
  rclcpp_action
  control_msgs
  sensor_msgs
  std_srvs
  trajectory_msgs
)

# Add executable with message files
add_executable(d1_controller
  src/d1_controller.cpp
  src/msg/ArmString_.cpp
  src/msg/PubServoInfo_.cpp
)

# Target dependencies for ROS2
ament_target_dependencies(d1_controller ${DEPENDENCY_LIST})

# Install targets
install(TARGETS
  d1_controller
  DESTINATION lib/${PROJECT_NAME}
)

# Install launch files and config
install(DIRECTORY
  launch
  config
  DESTINATION share/${PROJECT_NAME}/
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
