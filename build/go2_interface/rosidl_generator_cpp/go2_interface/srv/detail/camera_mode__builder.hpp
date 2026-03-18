// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from go2_interface:srv/CameraMode.idl
// generated code does not contain a copyright notice

#ifndef GO2_INTERFACE__SRV__DETAIL__CAMERA_MODE__BUILDER_HPP_
#define GO2_INTERFACE__SRV__DETAIL__CAMERA_MODE__BUILDER_HPP_

#include "go2_interface/srv/detail/camera_mode__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace go2_interface
{

namespace srv
{

namespace builder
{

class Init_CameraMode_Request_camera_mode
{
public:
  Init_CameraMode_Request_camera_mode()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::go2_interface::srv::CameraMode_Request camera_mode(::go2_interface::srv::CameraMode_Request::_camera_mode_type arg)
  {
    msg_.camera_mode = std::move(arg);
    return std::move(msg_);
  }

private:
  ::go2_interface::srv::CameraMode_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::go2_interface::srv::CameraMode_Request>()
{
  return go2_interface::srv::builder::Init_CameraMode_Request_camera_mode();
}

}  // namespace go2_interface


namespace go2_interface
{

namespace srv
{

namespace builder
{

class Init_CameraMode_Response_message
{
public:
  explicit Init_CameraMode_Response_message(::go2_interface::srv::CameraMode_Response & msg)
  : msg_(msg)
  {}
  ::go2_interface::srv::CameraMode_Response message(::go2_interface::srv::CameraMode_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::go2_interface::srv::CameraMode_Response msg_;
};

class Init_CameraMode_Response_success
{
public:
  Init_CameraMode_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CameraMode_Response_message success(::go2_interface::srv::CameraMode_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_CameraMode_Response_message(msg_);
  }

private:
  ::go2_interface::srv::CameraMode_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::go2_interface::srv::CameraMode_Response>()
{
  return go2_interface::srv::builder::Init_CameraMode_Response_success();
}

}  // namespace go2_interface

#endif  // GO2_INTERFACE__SRV__DETAIL__CAMERA_MODE__BUILDER_HPP_
