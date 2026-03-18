// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from go2_interface:srv/Go2Modes.idl
// generated code does not contain a copyright notice

#ifndef GO2_INTERFACE__SRV__DETAIL__GO2_MODES__BUILDER_HPP_
#define GO2_INTERFACE__SRV__DETAIL__GO2_MODES__BUILDER_HPP_

#include "go2_interface/srv/detail/go2_modes__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace go2_interface
{

namespace srv
{

namespace builder
{

class Init_Go2Modes_Request_request_data
{
public:
  Init_Go2Modes_Request_request_data()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::go2_interface::srv::Go2Modes_Request request_data(::go2_interface::srv::Go2Modes_Request::_request_data_type arg)
  {
    msg_.request_data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::go2_interface::srv::Go2Modes_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::go2_interface::srv::Go2Modes_Request>()
{
  return go2_interface::srv::builder::Init_Go2Modes_Request_request_data();
}

}  // namespace go2_interface


namespace go2_interface
{

namespace srv
{

namespace builder
{

class Init_Go2Modes_Response_reason
{
public:
  explicit Init_Go2Modes_Response_reason(::go2_interface::srv::Go2Modes_Response & msg)
  : msg_(msg)
  {}
  ::go2_interface::srv::Go2Modes_Response reason(::go2_interface::srv::Go2Modes_Response::_reason_type arg)
  {
    msg_.reason = std::move(arg);
    return std::move(msg_);
  }

private:
  ::go2_interface::srv::Go2Modes_Response msg_;
};

class Init_Go2Modes_Response_success
{
public:
  Init_Go2Modes_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Go2Modes_Response_reason success(::go2_interface::srv::Go2Modes_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_Go2Modes_Response_reason(msg_);
  }

private:
  ::go2_interface::srv::Go2Modes_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::go2_interface::srv::Go2Modes_Response>()
{
  return go2_interface::srv::builder::Init_Go2Modes_Response_success();
}

}  // namespace go2_interface

#endif  // GO2_INTERFACE__SRV__DETAIL__GO2_MODES__BUILDER_HPP_
