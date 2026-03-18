#include <geometry_msgs/msg/transform_stamped.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <cmath> // for sqrt

using std::placeholders::_1;

class OdomToTF : public rclcpp::Node
{
public:
  OdomToTF() : Node("odom_to_tf")
  {
    std::string odom_topic;
    frame_id_ = this->declare_parameter("frame_id", std::string(""));
    child_frame_id_ = this->declare_parameter("child_frame_id", std::string(""));
    odom_topic = this->declare_parameter("odom_topic", std::string("/odom/perfect"));
    RCLCPP_INFO(this->get_logger(), "odom_topic set to %s", odom_topic.c_str());
    inverse_tf_ = this->declare_parameter("inverse_tf_", false);
    use_original_timestamp_ = this->declare_parameter("use_original_timestamp", false);

    if (!frame_id_.empty())
      RCLCPP_INFO(this->get_logger(), "frame_id set to %s", frame_id_.c_str());
    else
      RCLCPP_INFO(this->get_logger(), "frame_id was not set. Using odom message frame_id.");

    if (!child_frame_id_.empty())
      RCLCPP_INFO(this->get_logger(), "child_frame_id set to %s", child_frame_id_.c_str());
    else
      RCLCPP_INFO(this->get_logger(), "child_frame_id was not set. Using odom message child_frame_id.");

    sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
        odom_topic,
        rclcpp::SensorDataQoS(),
        std::bind(&OdomToTF::odomCallback, this, _1));

    tfb_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
  }

private:
  std::string frame_id_, child_frame_id_;
  bool inverse_tf_, use_original_timestamp_;
  std::shared_ptr<tf2_ros::TransformBroadcaster> tfb_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr sub_;

  static geometry_msgs::msg::Quaternion invertQuaternion(
      const geometry_msgs::msg::Quaternion &q)
  {
    geometry_msgs::msg::Quaternion qi;
    // Inverse of a unit quaternion: conjugate
    qi.x = -q.x;
    qi.y = -q.y;
    qi.z = -q.z;
    qi.w = q.w;
    return qi;
  }

  void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg) const
  {
    geometry_msgs::msg::TransformStamped tfs_;

    if (use_original_timestamp_)
    {
      tfs_.header.stamp = msg->header.stamp;
    }
    else
    {
      tfs_.header.stamp = this->now();
    }

    if (!inverse_tf_)
    {
      tfs_.header.frame_id = !frame_id_.empty() ? frame_id_ : msg->header.frame_id;
      tfs_.child_frame_id = !child_frame_id_.empty() ? child_frame_id_ : msg->child_frame_id;
      tfs_.transform.translation.x = msg->pose.pose.position.x;
      tfs_.transform.translation.y = msg->pose.pose.position.y;
      tfs_.transform.translation.z = msg->pose.pose.position.z;
      tfs_.transform.rotation = msg->pose.pose.orientation;
    }
    else
    {
      // Inverse: swap parent/child, invert rotation, and translate
      tfs_.header.frame_id = !frame_id_.empty() ? frame_id_ : msg->child_frame_id;
      tfs_.child_frame_id = !child_frame_id_.empty() ? child_frame_id_ : msg->header.frame_id;

      geometry_msgs::msg::Quaternion q_inv = invertQuaternion(msg->pose.pose.orientation);

      // Rotate the translation vector by inverted rotation
      double x = msg->pose.pose.position.x;
      double y = msg->pose.pose.position.y;
      double z = msg->pose.pose.position.z;

      // Apply quaternion rotation: v' = q * v * q^-1
      double qx = q_inv.x, qy = q_inv.y, qz = q_inv.z, qw = q_inv.w;

      // Vector as quaternion
      double ix = qw * x + qy * z - qz * y;
      double iy = qw * y + qz * x - qx * z;
      double iz = qw * z + qx * y - qy * x;
      double iw = -qx * x - qy * y - qz * z;

      double rx = ix * qw + iw * -qx + iy * -qz - iz * -qy;
      double ry = iy * qw + iw * -qy + iz * -qx - ix * -qz;
      double rz = iz * qw + iw * -qz + ix * -qy - iy * -qx;

      tfs_.transform.translation.x = -rx;
      tfs_.transform.translation.y = -ry;
      tfs_.transform.translation.z = -rz;
      tfs_.transform.rotation = q_inv;
    }

    tfb_->sendTransform(tfs_);
  }
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<OdomToTF>());
  rclcpp::shutdown();
  return 0;
}