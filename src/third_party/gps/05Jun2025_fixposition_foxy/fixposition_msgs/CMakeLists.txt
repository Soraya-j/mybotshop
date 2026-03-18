# GENERAL ==============================================================================================================
cmake_minimum_required(VERSION 3.5)
project(fixposition_msgs)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(AMENT_DEPENDENCIES std_srvs 
                       std_msgs
                       )

# DEPENDENCIES =========================================================================================================
find_package(ament_cmake REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rclcpp REQUIRED)


rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Speed.msg"
  "msg/Gnsssats.msg"
  "msg/NMEA.msg"
  "msg/WheelSensor.msg"
  "msg/fpa/GNSSANT.msg"
  "msg/fpa/GNSSCORR.msg"
  "msg/fpa/LLH.msg"
  "msg/fpa/ODOMENU.msg"
  "msg/fpa/ODOMETRY.msg"
  "msg/fpa/ODOMSH.msg"
  "msg/fpa/TEXT.msg"
  "msg/nmea/GPGGA.msg"
  "msg/nmea/GPGLL.msg"
  "msg/nmea/GNGSA.msg"
  "msg/nmea/GPGST.msg"
  "msg/nmea/GXGSV.msg"
  "msg/nmea/GPHDT.msg"
  "msg/nmea/GPRMC.msg"
  "msg/nmea/GPVTG.msg"
  "msg/nmea/GPZDA.msg"
  DEPENDENCIES
  std_msgs
  nav_msgs
  geometry_msgs
  builtin_interfaces
)

# Make sure to export the rosidl runtime dependencies before ament_package
ament_export_dependencies(rosidl_default_runtime)

# Define the ament package for this project
ament_package()
