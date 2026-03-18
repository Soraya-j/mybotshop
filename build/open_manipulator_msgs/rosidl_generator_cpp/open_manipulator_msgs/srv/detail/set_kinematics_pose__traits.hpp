// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from open_manipulator_msgs:srv/SetKinematicsPose.idl
// generated code does not contain a copyright notice

#ifndef OPEN_MANIPULATOR_MSGS__SRV__DETAIL__SET_KINEMATICS_POSE__TRAITS_HPP_
#define OPEN_MANIPULATOR_MSGS__SRV__DETAIL__SET_KINEMATICS_POSE__TRAITS_HPP_

#include "open_manipulator_msgs/srv/detail/set_kinematics_pose__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

// Include directives for member types
// Member 'kinematics_pose'
#include "open_manipulator_msgs/msg/detail/kinematics_pose__traits.hpp"

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<open_manipulator_msgs::srv::SetKinematicsPose_Request>()
{
  return "open_manipulator_msgs::srv::SetKinematicsPose_Request";
}

template<>
inline const char * name<open_manipulator_msgs::srv::SetKinematicsPose_Request>()
{
  return "open_manipulator_msgs/srv/SetKinematicsPose_Request";
}

template<>
struct has_fixed_size<open_manipulator_msgs::srv::SetKinematicsPose_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<open_manipulator_msgs::srv::SetKinematicsPose_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<open_manipulator_msgs::srv::SetKinematicsPose_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<open_manipulator_msgs::srv::SetKinematicsPose_Response>()
{
  return "open_manipulator_msgs::srv::SetKinematicsPose_Response";
}

template<>
inline const char * name<open_manipulator_msgs::srv::SetKinematicsPose_Response>()
{
  return "open_manipulator_msgs/srv/SetKinematicsPose_Response";
}

template<>
struct has_fixed_size<open_manipulator_msgs::srv::SetKinematicsPose_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<open_manipulator_msgs::srv::SetKinematicsPose_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<open_manipulator_msgs::srv::SetKinematicsPose_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<open_manipulator_msgs::srv::SetKinematicsPose>()
{
  return "open_manipulator_msgs::srv::SetKinematicsPose";
}

template<>
inline const char * name<open_manipulator_msgs::srv::SetKinematicsPose>()
{
  return "open_manipulator_msgs/srv/SetKinematicsPose";
}

template<>
struct has_fixed_size<open_manipulator_msgs::srv::SetKinematicsPose>
  : std::integral_constant<
    bool,
    has_fixed_size<open_manipulator_msgs::srv::SetKinematicsPose_Request>::value &&
    has_fixed_size<open_manipulator_msgs::srv::SetKinematicsPose_Response>::value
  >
{
};

template<>
struct has_bounded_size<open_manipulator_msgs::srv::SetKinematicsPose>
  : std::integral_constant<
    bool,
    has_bounded_size<open_manipulator_msgs::srv::SetKinematicsPose_Request>::value &&
    has_bounded_size<open_manipulator_msgs::srv::SetKinematicsPose_Response>::value
  >
{
};

template<>
struct is_service<open_manipulator_msgs::srv::SetKinematicsPose>
  : std::true_type
{
};

template<>
struct is_service_request<open_manipulator_msgs::srv::SetKinematicsPose_Request>
  : std::true_type
{
};

template<>
struct is_service_response<open_manipulator_msgs::srv::SetKinematicsPose_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // OPEN_MANIPULATOR_MSGS__SRV__DETAIL__SET_KINEMATICS_POSE__TRAITS_HPP_
