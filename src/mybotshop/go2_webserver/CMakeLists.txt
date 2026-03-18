# ==============================================================================
# CMakeLists.txt for go2_webserver
# ==============================================================================
# Copyright (c) 2017-2026 MyBotShop
# All rights reserved.
#
# This package provides a web-based interface for remote robot operation,
# including VNC streaming, ROS bridge, and GPS waypoint management.
# ==============================================================================

cmake_minimum_required(VERSION 3.14)
project(go2_webserver VERSION 2.0.0 LANGUAGES CXX)

# ==============================================================================
# Build Configuration
# ==============================================================================

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ==============================================================================
# Dependencies
# ==============================================================================

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)

# ==============================================================================
# Python Package Installation
# ==============================================================================

ament_python_install_package(${PROJECT_NAME})

install(
  PROGRAMS
    scripts/webserver
  DESTINATION lib/${PROJECT_NAME}
)

# Install test script as 'ros_test'
install(
  PROGRAMS
    test/ros_test.py
  DESTINATION lib/${PROJECT_NAME}
  RENAME ros_test
)

# ==============================================================================
# Data Files Installation
# ==============================================================================

install(
  DIRECTORY
    config
    launch
    navi_outdoor_nogozones
    navi_outdoor_waypoints
    go2_webserver
    ssl
    test
  DESTINATION share/${PROJECT_NAME}
)

# ==============================================================================
# Testing
# ==============================================================================

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

# ==============================================================================
# Package Configuration
# ==============================================================================

ament_export_dependencies(rclcpp rclpy)
ament_package()
