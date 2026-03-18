// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from open_manipulator_msgs:srv/SetDrawingTrajectory.idl
// generated code does not contain a copyright notice

#ifndef OPEN_MANIPULATOR_MSGS__SRV__DETAIL__SET_DRAWING_TRAJECTORY__TRAITS_HPP_
#define OPEN_MANIPULATOR_MSGS__SRV__DETAIL__SET_DRAWING_TRAJECTORY__TRAITS_HPP_

#include "open_manipulator_msgs/srv/detail/set_drawing_trajectory__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<open_manipulator_msgs::srv::SetDrawingTrajectory_Request>()
{
  return "open_manipulator_msgs::srv::SetDrawingTrajectory_Request";
}

template<>
inline const char * name<open_manipulator_msgs::srv::SetDrawingTrajectory_Request>()
{
  return "open_manipulator_msgs/srv/SetDrawingTrajectory_Request";
}

template<>
struct has_fixed_size<open_manipulator_msgs::srv::SetDrawingTrajectory_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<open_manipulator_msgs::srv::SetDrawingTrajectory_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<open_manipulator_msgs::srv::SetDrawingTrajectory_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<open_manipulator_msgs::srv::SetDrawingTrajectory_Response>()
{
  return "open_manipulator_msgs::srv::SetDrawingTrajectory_Response";
}

template<>
inline const char * name<open_manipulator_msgs::srv::SetDrawingTrajectory_Response>()
{
  return "open_manipulator_msgs/srv/SetDrawingTrajectory_Response";
}

template<>
struct has_fixed_size<open_manipulator_msgs::srv::SetDrawingTrajectory_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<open_manipulator_msgs::srv::SetDrawingTrajectory_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<open_manipulator_msgs::srv::SetDrawingTrajectory_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<open_manipulator_msgs::srv::SetDrawingTrajectory>()
{
  return "open_manipulator_msgs::srv::SetDrawingTrajectory";
}

template<>
inline const char * name<open_manipulator_msgs::srv::SetDrawingTrajectory>()
{
  return "open_manipulator_msgs/srv/SetDrawingTrajectory";
}

template<>
struct has_fixed_size<open_manipulator_msgs::srv::SetDrawingTrajectory>
  : std::integral_constant<
    bool,
    has_fixed_size<open_manipulator_msgs::srv::SetDrawingTrajectory_Request>::value &&
    has_fixed_size<open_manipulator_msgs::srv::SetDrawingTrajectory_Response>::value
  >
{
};

template<>
struct has_bounded_size<open_manipulator_msgs::srv::SetDrawingTrajectory>
  : std::integral_constant<
    bool,
    has_bounded_size<open_manipulator_msgs::srv::SetDrawingTrajectory_Request>::value &&
    has_bounded_size<open_manipulator_msgs::srv::SetDrawingTrajectory_Response>::value
  >
{
};

template<>
struct is_service<open_manipulator_msgs::srv::SetDrawingTrajectory>
  : std::true_type
{
};

template<>
struct is_service_request<open_manipulator_msgs::srv::SetDrawingTrajectory_Request>
  : std::true_type
{
};

template<>
struct is_service_response<open_manipulator_msgs::srv::SetDrawingTrajectory_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // OPEN_MANIPULATOR_MSGS__SRV__DETAIL__SET_DRAWING_TRAJECTORY__TRAITS_HPP_
