cmake_minimum_required(VERSION 3.10)
project(go2_zt30)

# Use C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)   # Must be built with GStreamer support

# Executable
add_executable(rtsp_publisher src/rtsp_to_image.cpp)

target_include_directories(rtsp_publisher PRIVATE
  ${OpenCV_INCLUDE_DIRS}
)

# ament deps + OpenCV libs
ament_target_dependencies(rtsp_publisher
  rclcpp
  sensor_msgs
  cv_bridge
)
target_link_libraries(rtsp_publisher
  ${OpenCV_LIBS}
)

# Warnings (optional)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(rtsp_publisher PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install C++ node
install(TARGETS rtsp_publisher
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

# Install resources
install(DIRECTORY config launch
  DESTINATION share/${PROJECT_NAME}
)

# Install Python modules
ament_python_install_package(${PROJECT_NAME})


# Install Python executables
install(PROGRAMS scripts/driver
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
