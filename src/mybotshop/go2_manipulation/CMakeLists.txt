cmake_minimum_required(VERSION 3.5)
project(go2_manipulation)
add_compile_options(-std=c++17)

set(AMENT_DEPENDENCIES rclpy
                       geometry_msgs
                       sensor_msgs
                       tf2
                       tf2_ros
                       std_srvs 
                       std_msgs
)

set (DEPENDENCY_LIST std_msgs
                     std_srvs
)

set(CMAKE_CXX_FLAGS "-O3 -fPIC")

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclpy REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)

install(PROGRAMS  go2_manipulation/openmanipulator_bridge
                  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch config
        DESTINATION share/${PROJECT_NAME}
)

ament_python_install_package(${PROJECT_NAME})
ament_package()