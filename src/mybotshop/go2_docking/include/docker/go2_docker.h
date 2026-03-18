#ifndef GO2_DOCKER_H
#define GO2_DOCKER_H

#include <vector>
#include <cmath>
#include <string>
#include <chrono> 

#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_sensor_msgs/tf2_sensor_msgs.h"

#include "std_srvs/srv/trigger.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "apriltag_msgs/msg/april_tag_detection_array.hpp"

class Go2Docker : public rclcpp::Node
{
public:
    Go2Docker();
    ~Go2Docker();

private:
    // ROS2
    rclcpp::Subscription<apriltag_msgs::msg::AprilTagDetectionArray>::SharedPtr sub_apriltag_detection_;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr srv_docking_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_cmd_vel_;
    rclcpp::TimerBase::SharedPtr docking_timer_; 

    // Parameters
    tf2_ros::Buffer tf_buffer_;
    tf2_ros::TransformListener tf_listener_;
    std::string param_camera_type_;
    std::string param_april_tag_topic_;
    std::string param_cmd_vel_topic_;
    std::string target_frame_id_;
    std::string base_frame_id_;
    double target_distance_;
    double linear_speed_;
    double angular_speed_;
    double robot_linear_x_;
    double robot_linear_y_;
    double robot_linear_z_;
    double robot_angle_;
    double docking_x_;
    double docking_y_;
    double docking_z_;
    double docking_a_;
    double docking_tolerance_angle_;
    double docking_tolerance_translation_;
    int timeout_;

    // Control variables
    bool is_docking_ = false; 
    bool is_target_detected_{false};
    geometry_msgs::msg::TransformStamped target_transform_;
    std::chrono::time_point<std::chrono::steady_clock> start_time;

    // Functions
    void callback_apriltag_detection(const apriltag_msgs::msg::AprilTagDetectionArray::SharedPtr msg);
    void callback_docking(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                          std::shared_ptr<std_srvs::srv::Trigger::Response> response);

    void docking_manuever();
    void approach_dock();
    void stop_robot();
    bool is_within_target_distance();
     void quaternionToEuler(double x, double y, double z, double w, double &roll, double &pitch, double &yaw);
    std::string colorize(const std::string &text, const std::string &color) const;
};

#endif // GO2_DOCKER_H
