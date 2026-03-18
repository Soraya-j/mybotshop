cmake_minimum_required(VERSION 3.5)
project(go2_platform)
add_compile_options(-std=c++17)

# Build settings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIC")

# Find dependencies
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(rclpy REQUIRED)
find_package(rclcpp REQUIRED)
find_package(go2_interface REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(unitree_go REQUIRED)
find_package(unitree_api REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(rosbag2_cpp REQUIRED)
find_package(camera_info_manager REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(PCL REQUIRED COMPONENTS common filters)
find_package(pcl_conversions REQUIRED)

# OpenCV and GStreamer for video processing
find_package(OpenCV REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(PkgConfig REQUIRED)

# GStreamer packages
pkg_check_modules(GST REQUIRED 
    gstreamer-1.0 
    gstreamer-video-1.0 
    gstreamer-app-1.0)

# Set dependencies
set (DEPENDENCY_LIST unitree_go
                     unitree_api
                     rclcpp
                     rclpy
                     geometry_msgs
                     sensor_msgs
                     nav_msgs
                     tf2
                     tf2_ros
                     std_srvs
                     std_msgs
                     go2_interface
                     rosbag2_cpp
                     camera_info_manager
                     ament_index_cpp
                     pcl_conversions)

# Include directories
include_directories(
                    include
                    /usr/include/eigen3
                    /usr/local/include/ddscxx
                    /usr/local/include/iceoryx/v2.0.2
                    include/qre
                    include/unitree
                    ${OpenCV_INCLUDE_DIRS}
                    ${cv_bridge_INCLUDE_DIRS}
                    ${GST_INCLUDE_DIRS}
                    ${PCL_INCLUDE_DIRS})

# Link libraries and directories
link_libraries(unitree_sdk2 ddsc ddscxx rt pthread)
link_directories(src ${GST_LIBRARY_DIRS})

# Executables
add_executable(go2_scan src/go2_scan_node.cpp)
add_executable(go2_highroscontrol src/go2_highroscontrol.cpp )
add_executable(go2_lowroscontrol src/go2_lowlevel_example.cpp)
add_executable(go2_video_publisher src/go2_video_publisher.cpp)

# Linking dependencies
ament_target_dependencies(go2_scan ${DEPENDENCY_LIST})
target_link_libraries(go2_scan ${PCL_LIBRARIES})
ament_target_dependencies(go2_highroscontrol ${DEPENDENCY_LIST})
ament_target_dependencies(go2_lowroscontrol ${DEPENDENCY_LIST})
ament_target_dependencies(go2_video_publisher rclcpp sensor_msgs camera_info_manager ament_index_cpp)

# Link Go2 video publisher with GStreamer and OpenCV
target_link_libraries(go2_video_publisher
  ${OpenCV_LIBRARIES}
  ${GST_LIBRARIES}
)

# Add GStreamer compile options
target_compile_options(go2_video_publisher PRIVATE ${GST_CFLAGS_OTHER})

# Install programs
install(PROGRAMS  go2_platform/go2_statepublisher
                  DESTINATION lib/${PROJECT_NAME}
)

# Install executables
install(TARGETS  go2_highroscontrol
                 go2_lowroscontrol
                 go2_scan
                 go2_video_publisher
                 DESTINATION lib/${PROJECT_NAME})

# Install directories
install(DIRECTORY launch config
        DESTINATION share/${PROJECT_NAME}
)

# Python package
ament_python_install_package(${PROJECT_NAME})
ament_package()