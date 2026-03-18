cmake_minimum_required(VERSION 3.5)
project(go2_docking)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(apriltag_msgs REQUIRED)
find_package(std_srvs REQUIRED)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wdeprecated -fPIC -Wshadow -Wnull-dereference)

set(DEPENDENCY_LIST
  rclcpp
  std_msgs
  sensor_msgs
  nav2_msgs
  nav_msgs
  geometry_msgs
  tf2_ros
  tf2_geometry_msgs
  apriltag_msgs
  std_srvs
)

# -------------------------------------------------------- CPP files
include_directories(include/docker)

add_executable(dock_navi src/dock_navi_node.cpp)

ament_target_dependencies(dock_navi ${DEPENDENCY_LIST})

install(TARGETS
  dock_navi
  DESTINATION lib/${PROJECT_NAME})

# -------------------------------------------------------- Python files
install(PROGRAMS scripts/april_ros_pub
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch config DESTINATION share/${PROJECT_NAME})

ament_export_include_directories()
ament_export_dependencies(${dependencies})

# -------------------------------------------------------- Build
ament_python_install_package(${PROJECT_NAME})
ament_package()
