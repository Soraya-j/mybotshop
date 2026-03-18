#ifndef GO2_SCAN_H
#define GO2_SCAN_H

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_sensor_msgs/tf2_sensor_msgs.h"
#include "sensor_msgs/point_cloud2_iterator.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl_conversions/pcl_conversions.h>

#include <boost/make_shared.hpp>
#include <deque>
#include <mutex>
#include <cmath>
#include <string>

// PCL 1.10 uses boost::shared_ptr, not std::shared_ptr
using PclCloudPtr = pcl::PointCloud<pcl::PointXYZ>::Ptr;

struct StampedCloud
{
    PclCloudPtr cloud;
    rclcpp::Time stamp;
};

class Go2Scanner : public rclcpp::Node {
public:
    Go2Scanner();

private:
    // ROS2
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr sub_lidar_;
    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr pub_scan_;
    rclcpp::TimerBase::SharedPtr timer_scan_;

    // TF
    tf2_ros::Buffer tf_buffer_;
    tf2_ros::TransformListener tf_listener_;

    // Parameters
    std::string param_target_frame_;
    std::string param_pcd_sub_;
    std::string param_scan_pub_;
    double param_min_height_;
    double param_max_height_;
    double param_scan_time_;
    double param_range_min_;
    double param_range_max_;
    bool param_use_inf_;
    double param_inf_epsilon_;
    double param_angle_increment_;
    double param_accumulation_window_;
    double param_voxel_leaf_size_;
    double param_transform_tolerance_;
    int param_publisher_freq_;

    // Accumulator
    std::deque<StampedCloud> cloud_buffer_;
    std::mutex buffer_mutex_;

    // Functions
    void update_scan();
    void cloud_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg);
    std::string colorize(const std::string &text, const std::string &color) const;
};

#endif // GO2_SCAN_H
