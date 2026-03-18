#include "go2_platform.h"

Go2Scanner::Go2Scanner(): Node("go2_scan"), tf_buffer_(this->get_clock()), tf_listener_(tf_buffer_)
{
    RCLCPP_INFO(this->get_logger(), "%s", colorize("Initializing Go2 Scan Publisher!", "orange").c_str());

    // Declare parameters
    this->declare_parameter<std::string>("target_frame", "lidar_link");
    this->declare_parameter<std::string>("pcd_sub", "/utlidar/cloud");
    this->declare_parameter<std::string>("scan_pub", "/go2/scan");
    this->declare_parameter<double>("min_height", 0.1);
    this->declare_parameter<double>("max_height", 0.3);
    this->declare_parameter<double>("scan_time", 0.005);
    this->declare_parameter<double>("range_min", 0.0);
    this->declare_parameter<double>("range_max", 10.0);
    this->declare_parameter<bool>("use_inf", true);
    this->declare_parameter<double>("inf_epsilon", 1.0);
    this->declare_parameter<double>("angle_increment", 0.01);
    this->declare_parameter<double>("accumulation_window", 0.5);
    this->declare_parameter<double>("voxel_leaf_size", 0.03);
    this->declare_parameter<double>("transform_tolerance", 0.1);
    this->declare_parameter<int>("publisher_frequency", 50);

    // Get parameters
    param_target_frame_ = this->get_parameter("target_frame").as_string();
    param_pcd_sub_ = this->get_parameter("pcd_sub").as_string();
    param_scan_pub_ = this->get_parameter("scan_pub").as_string();
    param_min_height_ = this->get_parameter("min_height").as_double();
    param_max_height_ = this->get_parameter("max_height").as_double();
    param_scan_time_ = this->get_parameter("scan_time").as_double();
    param_range_min_ = this->get_parameter("range_min").as_double();
    param_range_max_ = this->get_parameter("range_max").as_double();
    param_use_inf_ = this->get_parameter("use_inf").as_bool();
    param_inf_epsilon_ = this->get_parameter("inf_epsilon").as_double();
    param_angle_increment_ = this->get_parameter("angle_increment").as_double();
    param_accumulation_window_ = this->get_parameter("accumulation_window").as_double();
    param_voxel_leaf_size_ = this->get_parameter("voxel_leaf_size").as_double();
    param_transform_tolerance_ = this->get_parameter("transform_tolerance").as_double();
    param_publisher_freq_ = this->get_parameter("publisher_frequency").as_int();

    RCLCPP_INFO(this->get_logger(), "%s", colorize("target_frame: " + param_target_frame_, "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("pcd_sub: " + param_pcd_sub_, "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("scan_pub: " + param_scan_pub_, "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("min_height: " + std::to_string(param_min_height_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("max_height: " + std::to_string(param_max_height_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("scan_time: " + std::to_string(param_scan_time_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("range_min: " + std::to_string(param_range_min_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("range_max: " + std::to_string(param_range_max_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("use_inf: " + std::to_string(param_use_inf_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("inf_epsilon: " + std::to_string(param_inf_epsilon_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("angle_increment: " + std::to_string(param_angle_increment_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("accumulation_window: " + std::to_string(param_accumulation_window_) + " s", "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("voxel_leaf_size: " + std::to_string(param_voxel_leaf_size_) + " m", "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("transform_tolerance: " + std::to_string(param_transform_tolerance_), "blue").c_str());
    RCLCPP_INFO(this->get_logger(), "%s", colorize("publisher_frequency: " + std::to_string(param_publisher_freq_), "blue").c_str());

    // Initialize ROS2 Publishers and Subscribers
    std::this_thread::sleep_for(std::chrono::seconds(3));

    sub_lidar_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
        param_pcd_sub_, 10, std::bind(&Go2Scanner::cloud_callback, this, std::placeholders::_1));

    pub_scan_ = this->create_publisher<sensor_msgs::msg::LaserScan>(param_scan_pub_, 10);
    timer_scan_ = this->create_wall_timer(
        std::chrono::milliseconds(1000 / param_publisher_freq_),
        std::bind(&Go2Scanner::update_scan, this));
}

void Go2Scanner::cloud_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
{
    // Transform the incoming cloud to target_frame using the cloud's own timestamp.
    // This ensures correct spatial alignment regardless of robot motion.
    sensor_msgs::msg::PointCloud2 cloud_transformed;

    if (param_target_frame_ != msg->header.frame_id)
    {
        try
        {
            auto transform = tf_buffer_.lookupTransform(
                param_target_frame_, msg->header.frame_id,
                tf2::timeFromSec(rclcpp::Time(msg->header.stamp).seconds()),
                tf2::durationFromSec(param_transform_tolerance_));

            tf2::doTransform(*msg, cloud_transformed, transform);
        }
        catch (tf2::TransformException &ex)
        {
            RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 2000,
                "Transform error: %s", ex.what());
            return;
        }
    }
    else
    {
        cloud_transformed = *msg;
    }

    // Convert to PCL, filtering by height during conversion to reduce memory
    PclCloudPtr pcl_cloud(new pcl::PointCloud<pcl::PointXYZ>());
    pcl_cloud->reserve(cloud_transformed.width * cloud_transformed.height);

    for (sensor_msgs::PointCloud2ConstIterator<float> iter_x(cloud_transformed, "x"),
            iter_y(cloud_transformed, "y"), iter_z(cloud_transformed, "z");
            iter_x != iter_x.end(); ++iter_x, ++iter_y, ++iter_z)
    {
        const float z = *iter_z;
        if (std::isnan(*iter_x) || std::isnan(*iter_y) || std::isnan(z))
            continue;
        if (z < param_min_height_ || z > param_max_height_)
            continue;

        pcl_cloud->push_back(pcl::PointXYZ(*iter_x, *iter_y, z));
    }

    if (pcl_cloud->empty())
        return;

    StampedCloud entry;
    entry.cloud = pcl_cloud;
    entry.stamp = rclcpp::Time(msg->header.stamp);

    std::lock_guard<std::mutex> lock(buffer_mutex_);
    cloud_buffer_.push_back(std::move(entry));
}

void Go2Scanner::update_scan()
{
    // Evict clouds older than the accumulation window
    const rclcpp::Time now = this->get_clock()->now();
    const rclcpp::Duration window = rclcpp::Duration::from_seconds(param_accumulation_window_);
    const rclcpp::Time cutoff = (now - window);

    // Merge all buffered clouds into one
    PclCloudPtr merged(new pcl::PointCloud<pcl::PointXYZ>());

    {
        std::lock_guard<std::mutex> lock(buffer_mutex_);

        // Evict expired entries from front
        while (!cloud_buffer_.empty() && cloud_buffer_.front().stamp < cutoff)
        {
            cloud_buffer_.pop_front();
        }

        if (cloud_buffer_.empty())
            return;

        // Pre-compute total size for reservation
        size_t total_points = 0;
        for (const auto &entry : cloud_buffer_)
        {
            total_points += entry.cloud->size();
        }
        merged->reserve(total_points);

        for (const auto &entry : cloud_buffer_)
        {
            *merged += *(entry.cloud);
        }
    }

    // Voxel grid downsample to avoid redundant point density
    PclCloudPtr filtered(new pcl::PointCloud<pcl::PointXYZ>());
    if (param_voxel_leaf_size_ > 0.0)
    {
        pcl::VoxelGrid<pcl::PointXYZ> voxel;
        voxel.setInputCloud(merged);
        voxel.setLeafSize(
            static_cast<float>(param_voxel_leaf_size_),
            static_cast<float>(param_voxel_leaf_size_),
            static_cast<float>(param_voxel_leaf_size_));
        voxel.filter(*filtered);
    }
    else
    {
        filtered = merged;
    }

    // Project filtered cloud to LaserScan
    auto scan_msg = std::make_shared<sensor_msgs::msg::LaserScan>();

    scan_msg->header.stamp = now;
    scan_msg->header.frame_id = param_target_frame_;
    scan_msg->angle_min = -M_PI;
    scan_msg->angle_max = M_PI;
    scan_msg->angle_increment = param_angle_increment_;
    scan_msg->time_increment = 0.0;
    scan_msg->scan_time = param_scan_time_;
    scan_msg->range_min = param_range_min_;
    scan_msg->range_max = param_range_max_;

    const size_t ranges_size = static_cast<size_t>(
        std::ceil((scan_msg->angle_max - scan_msg->angle_min) / scan_msg->angle_increment));

    if (param_use_inf_)
    {
        scan_msg->ranges.assign(ranges_size, std::numeric_limits<float>::infinity());
    }
    else
    {
        scan_msg->ranges.assign(ranges_size, static_cast<float>(param_range_max_ + param_inf_epsilon_));
    }

    for (const auto &pt : filtered->points)
    {
        const float range_val = std::hypot(pt.x, pt.y);
        if (range_val < param_range_min_ || range_val > param_range_max_)
            continue;

        const float angle = std::atan2(pt.y, pt.x);
        if (angle < scan_msg->angle_min || angle > scan_msg->angle_max)
            continue;

        const int index = static_cast<int>((angle - scan_msg->angle_min) / scan_msg->angle_increment);
        if (index < 0 || index >= static_cast<int>(ranges_size))
            continue;

        if (range_val < scan_msg->ranges[index])
        {
            scan_msg->ranges[index] = range_val;
        }
    }

    RCLCPP_INFO_ONCE(this->get_logger(), "%s", colorize("GO2 Lidar Data Received", "green").c_str());
    pub_scan_->publish(*scan_msg);
}

std::string Go2Scanner::colorize(const std::string &text, const std::string &color) const
{
    std::string color_code;
    if (color == "orange") {
        color_code = "\033[38;5;214m";
    } else if (color == "blue") {
        color_code = "\033[34m";
    } else if (color == "green") {
        color_code = "\033[92m";
    } else if (color == "red") {
        color_code = "\033[31m";
    } else if (color == "purple") {
        color_code = "\033[35m";
    } else {
        color_code = "\033[0m";
    }
    return color_code + text + "\033[0m";
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Go2Scanner>());
    rclcpp::shutdown();
    return 0;
}
