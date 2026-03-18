// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from go2_interface:msg/CameraGimbalCmd.idl
// generated code does not contain a copyright notice

#ifndef GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__BUILDER_HPP_
#define GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__BUILDER_HPP_

#include "go2_interface/msg/detail/camera_gimbal_cmd__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace go2_interface
{

namespace msg
{

namespace builder
{

class Init_CameraGimbalCmd_gimbal_mode
{
public:
  explicit Init_CameraGimbalCmd_gimbal_mode(::go2_interface::msg::CameraGimbalCmd & msg)
  : msg_(msg)
  {}
  ::go2_interface::msg::CameraGimbalCmd gimbal_mode(::go2_interface::msg::CameraGimbalCmd::_gimbal_mode_type arg)
  {
    msg_.gimbal_mode = std::move(arg);
    return std::move(msg_);
  }

private:
  ::go2_interface::msg::CameraGimbalCmd msg_;
};

class Init_CameraGimbalCmd_pitch
{
public:
  explicit Init_CameraGimbalCmd_pitch(::go2_interface::msg::CameraGimbalCmd & msg)
  : msg_(msg)
  {}
  Init_CameraGimbalCmd_gimbal_mode pitch(::go2_interface::msg::CameraGimbalCmd::_pitch_type arg)
  {
    msg_.pitch = std::move(arg);
    return Init_CameraGimbalCmd_gimbal_mode(msg_);
  }

private:
  ::go2_interface::msg::CameraGimbalCmd msg_;
};

class Init_CameraGimbalCmd_yaw
{
public:
  Init_CameraGimbalCmd_yaw()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CameraGimbalCmd_pitch yaw(::go2_interface::msg::CameraGimbalCmd::_yaw_type arg)
  {
    msg_.yaw = std::move(arg);
    return Init_CameraGimbalCmd_pitch(msg_);
  }

private:
  ::go2_interface::msg::CameraGimbalCmd msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::go2_interface::msg::CameraGimbalCmd>()
{
  return go2_interface::msg::builder::Init_CameraGimbalCmd_yaw();
}

}  // namespace go2_interface

#endif  // GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__BUILDER_HPP_
