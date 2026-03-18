#include "go2_docker.h"

Go2Docker::Go2Docker()
    : Node("go2_docker"),
      tf_buffer_(this->get_clock()),
      tf_listener_(tf_buffer_)
{

    RCLCPP_INFO(this->get_logger(), "%s", colorize("Initializing Go2 Simple Docking Server!", "orange").c_str());

    this->declare_parameter<std::string>("camera_type", "go2");
    this->declare_parameter<std::string>("april_tag_topic", "/apriltag_detections");
    this->declare_parameter<std::string>("cmd_vel_topic", "/cmd_vel");
    this->declare_parameter<std::string>("detection_link", "tag_1");
    this->declare_parameter<std::string>("base_link", "trunk");
    this->declare_parameter<double>("linear_speed", 0.1);
    this->declare_parameter<double>("angular_speed", 0.1);
    this->declare_parameter<double>("docking_x", 0.0);
    this->declare_parameter<double>("docking_y", 0.0);
    this->declare_parameter<double>("docking_z", 0.0);
    this->declare_parameter<double>("docking_a", 0.0);
    this->declare_parameter<double>("docking_tolerance_angle", 0.1);
    this->declare_parameter<double>("docking_tolerance_translation", 0.1);
    this->declare_parameter<int>("timeout", 30.0);

    this->get_parameter("camera_type", param_camera_type_);
    this->get_parameter("april_tag_topic", param_april_tag_topic_);
    this->get_parameter("cmd_vel_topic", param_cmd_vel_topic_);
    this->get_parameter("detection_link", target_frame_id_);
    this->get_parameter("base_link", base_frame_id_);
    this->get_parameter("linear_speed", linear_speed_);
    this->get_parameter("angular_speed", angular_speed_);
    this->get_parameter("docking_x", docking_x_);
    this->get_parameter("docking_y", docking_y_);
    this->get_parameter("docking_z", docking_z_);
    this->get_parameter("docking_a", docking_a_);
    this->get_parameter("docking_tolerance_angle", docking_tolerance_angle_);
    this->get_parameter("docking_tolerance_translation", docking_tolerance_translation_);
    this->get_parameter("timeout", timeout_);

    RCLCPP_INFO(this->get_logger(), "%s", colorize("camera_type: " + param_camera_type_, "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("april_tag_topic: " + param_april_tag_topic_, "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("cmd_vel_topic: " + param_cmd_vel_topic_, "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("detection_link: " + target_frame_id_, "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("base_link: " + base_frame_id_, "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("linear_speed: " + std::to_string(linear_speed_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("angular_speed: " + std::to_string(angular_speed_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("docking_x: " + std::to_string(docking_x_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("docking_y: " + std::to_string(docking_y_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("docking_z: " + std::to_string(docking_z_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("docking_a: " + std::to_string(docking_a_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("docking_tolerance_angle: " + std::to_string(docking_tolerance_angle_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("docking_tolerance_translation: " + std::to_string(docking_tolerance_translation_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("timeout: " + std::to_string(timeout_), "blue").c_str());

    sub_apriltag_detection_ = this->create_subscription<apriltag_msgs::msg::AprilTagDetectionArray>(
        param_april_tag_topic_, 10,
        std::bind(&Go2Docker::callback_apriltag_detection, this, std::placeholders::_1));

    pub_cmd_vel_ = this->create_publisher<geometry_msgs::msg::Twist>(param_cmd_vel_topic_, 10);

    srv_docking_ = this->create_service<std_srvs::srv::Trigger>(
        "dock", std::bind(&Go2Docker::callback_docking, this, std::placeholders::_1, std::placeholders::_2));
}

Go2Docker::~Go2Docker() {}

void Go2Docker::callback_docking(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                                 std::shared_ptr<std_srvs::srv::Trigger::Response> response)
{
    (void)request;
    RCLCPP_INFO(this->get_logger(), "%s", colorize("Docking service called!", "cyan").c_str());
    start_time = std::chrono::steady_clock::now(); // Start the timer
    if (!is_docking_)
    {

        is_docking_ = true; // Set docking state
        docking_timer_ = this->create_wall_timer(
            std::chrono::milliseconds(50),                  // 50 Hz
            std::bind(&Go2Docker::docking_manuever, this)); // Call the docking function
    }

    response->success = true;
}

void Go2Docker::docking_manuever()
{
    auto elapsed_time = std::chrono::steady_clock::now() - start_time;

    if (elapsed_time > std::chrono::seconds(timeout_))
    {
        RCLCPP_INFO(this->get_logger(), "%s", colorize("Docker time exceeded. Configuration timeout: " + std::to_string(timeout_) + " seconds", "red").c_str());
        start_time = std::chrono::steady_clock::now(); // Reset timer for the next docking attempt
        stop_robot();
        docking_timer_->cancel(); // Stop the timer
        is_docking_ = false;      // Reset docking state
        return;                   // Exit the function
    }

    if (is_target_detected_)
    {
        if (!is_within_target_distance())
        {
            approach_dock();
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "%s", colorize("Target reached!", "green").c_str());
            stop_robot();
            docking_timer_->cancel();                      // Stop the timer
            is_docking_ = false;                           // Reset docking state
            start_time = std::chrono::steady_clock::now(); // Reset timer for the next docking attempt
        }
    }
}

void Go2Docker::stop_robot()
{
    geometry_msgs::msg::Twist twist_msg;
    twist_msg.linear.x = 0.0;
    twist_msg.linear.y = 0.0;
    twist_msg.linear.z = 0.0;
    twist_msg.angular.z = 0.0;
    pub_cmd_vel_->publish(twist_msg);
}

void Go2Docker::callback_apriltag_detection(const apriltag_msgs::msg::AprilTagDetectionArray::SharedPtr msg)
{
    if (!msg->detections.empty())
    {
        try
        {
            target_transform_ = tf_buffer_.lookupTransform(base_frame_id_, target_frame_id_, tf2::TimePointZero);
            is_target_detected_ = true;
            RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 20000, "%s",
                                 colorize("Docking frame id detected: " + target_frame_id_, "yellow").c_str());
        }
        catch (tf2::TransformException &ex)
        {
            RCLCPP_WARN(this->get_logger(), "Could not transform: %s", ex.what());
            is_target_detected_ = false;
        }
    }
    else
    {
        is_target_detected_ = false;
    }
}

void Go2Docker::approach_dock()
{
    geometry_msgs::msg::Twist twist_msg;

    // Reduced speed
    double remaining_distance_x = std::abs(robot_linear_x_ - docking_x_);
    double remaining_distance_y = std::abs(robot_linear_y_ - docking_y_);

    // Set angular velocity to align with x-axis
    double roll, pitch, yaw;
    tf2::Quaternion q(target_transform_.transform.rotation.x,
                      target_transform_.transform.rotation.y,
                      target_transform_.transform.rotation.z,
                      target_transform_.transform.rotation.w);
    tf2::Matrix3x3 m(q);
    m.getRPY(roll, pitch, yaw);

    // Use this to find out yaw for custom cameras
    // RCLCPP_INFO(this->get_logger(), "%s", colorize("The roll is: " + std::to_string(roll), "blue").c_str());
    // RCLCPP_INFO(this->get_logger(), "%s", colorize("The pitch is: " + std::to_string(pitch), "blue").c_str());
    // RCLCPP_INFO(this->get_logger(), "%s", colorize("The yaw is: " + std::to_string(yaw), "blue").c_str());

    if (param_camera_type_ == "go2")
    {
        robot_angle_ = pitch;
    }
    else
    {
        robot_angle_ = yaw;
    }
    if (robot_angle_ > docking_a_ + docking_tolerance_angle_)
    {
        twist_msg.angular.z = +angular_speed_;
    }
    else if (robot_angle_ < docking_a_ - docking_tolerance_angle_)
    {
        twist_msg.angular.z = -angular_speed_;
    }

    // Align the robot to the target y
    if (robot_linear_y_ > docking_y_ + docking_tolerance_translation_)
    {
        twist_msg.linear.y = std::min(linear_speed_, remaining_distance_y); // Move forward (positive y direction)
    }
    else if (robot_linear_y_ < docking_y_ - docking_tolerance_translation_)
    {
        twist_msg.linear.y = -std::min(linear_speed_, remaining_distance_y); // Move backward (negative y direction)
    }
    else
    {
        twist_msg.linear.y = 0.0; // Stop when within tolerance
    }

    // Move the robot forward
    if (robot_linear_x_ > docking_x_ + docking_tolerance_translation_)
    {
        twist_msg.linear.x = std::min(linear_speed_, remaining_distance_x); // Move forward
    }
    else if (robot_linear_x_ < docking_x_ - docking_tolerance_translation_)
    {
        twist_msg.linear.x = -std::min(linear_speed_, remaining_distance_x); // Move backward (negative y direction)
    }
    else
    {
        twist_msg.linear.x = 0.0;
    }
    pub_cmd_vel_->publish(twist_msg);
}

bool Go2Docker::is_within_target_distance()
{
    bool distance = false;

    robot_linear_x_ = target_transform_.transform.translation.x;
    robot_linear_y_ = target_transform_.transform.translation.y;
    robot_linear_z_ = target_transform_.transform.translation.z;

    if ((std::abs(robot_linear_x_ - docking_x_) <= docking_tolerance_translation_) &&
        (std::abs(robot_linear_y_ - docking_y_) <= docking_tolerance_translation_) &&
        // (std::abs(robot_linear_z_ - docking_z_) <= docking_tolerance_translation_)&&
        (std::abs(robot_angle_ - docking_a_) <= docking_tolerance_angle_))
    {
        distance = true;
    }

    RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 1000, "%s", colorize("Robot x is: " + std::to_string(robot_linear_x_) + " docking x is: " + std::to_string(docking_x_), "purple").c_str());
    RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 1000, "%s", colorize("Robot y is: " + std::to_string(robot_linear_y_) + " docking y is: " + std::to_string(docking_y_), "purple").c_str());
    RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 1000, "%s", colorize("Robot angle is: " + std::to_string(robot_angle_) + " docking angle is: " + std::to_string(docking_a_) + "\n", "purple").c_str());

    return distance;
}

void Go2Docker::quaternionToEuler(double x, double y, double z, double w, double &roll, double &pitch, double &yaw)
{
    // Compute roll
    roll = std::atan2(2.0 * (w * x + y * z), 1.0 - 2.0 * (x * x + y * y));

    // Compute pitch
    double sinp = 2.0 * (w * y - z * x);
    if (std::abs(sinp) >= 1.0)
        pitch = std::copysign(M_PI / 2.0, sinp); // Use 90 degrees if out of range
    else
        pitch = std::asin(sinp);

    // Compute yaw
    yaw = std::atan2(2.0 * (w * z + x * y), 1.0 - 2.0 * (y * y + z * z));

    // Convert to degrees
    roll = roll * 180.0 / M_PI;
    pitch = pitch * 180.0 / M_PI;
    yaw = yaw * 180.0 / M_PI;
}

std::string Go2Docker::colorize(const std::string &text, const std::string &color) const
{
    std::string color_code;
    if (color == "orange")
    {
        color_code = "\033[38;5;214m";
    }
    else if (color == "blue")
    {
        color_code = "\033[34m";
    }
    else if (color == "green")
    {
        color_code = "\033[92m";
    }
    else if (color == "red")
    {
        color_code = "\033[31m";
    }
    else if (color == "purple")
    {
        color_code = "\033[35m";
    }
    else if (color == "yellow")
    {
        color_code = "\033[33m";
    }
    else if (color == "cyan")
    {
        color_code = "\033[36m";
    }
    else
    {
        color_code = "\033[0m";
    }
    return color_code + text + "\033[0m";
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Go2Docker>();
    rclcpp::spin(node);
    RCLCPP_INFO(node->get_logger(), "\x1B[31mTerminating GO2 docking server\x1B[0m");
    rclcpp::shutdown();
    return 0;
}