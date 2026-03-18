// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from open_manipulator_msgs:srv/SetActuatorState.idl
// generated code does not contain a copyright notice

#ifndef OPEN_MANIPULATOR_MSGS__SRV__DETAIL__SET_ACTUATOR_STATE__TRAITS_HPP_
#define OPEN_MANIPULATOR_MSGS__SRV__DETAIL__SET_ACTUATOR_STATE__TRAITS_HPP_

#include "open_manipulator_msgs/srv/detail/set_actuator_state__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<open_manipulator_msgs::srv::SetActuatorState_Request>()
{
  return "open_manipulator_msgs::srv::SetActuatorState_Request";
}

template<>
inline const char * name<open_manipulator_msgs::srv::SetActuatorState_Request>()
{
  return "open_manipulator_msgs/srv/SetActuatorState_Request";
}

template<>
struct has_fixed_size<open_manipulator_msgs::srv::SetActuatorState_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<open_manipulator_msgs::srv::SetActuatorState_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<open_manipulator_msgs::srv::SetActuatorState_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<open_manipulator_msgs::srv::SetActuatorState_Response>()
{
  return "open_manipulator_msgs::srv::SetActuatorState_Response";
}

template<>
inline const char * name<open_manipulator_msgs::srv::SetActuatorState_Response>()
{
  return "open_manipulator_msgs/srv/SetActuatorState_Response";
}

template<>
struct has_fixed_size<open_manipulator_msgs::srv::SetActuatorState_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<open_manipulator_msgs::srv::SetActuatorState_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<open_manipulator_msgs::srv::SetActuatorState_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<open_manipulator_msgs::srv::SetActuatorState>()
{
  return "open_manipulator_msgs::srv::SetActuatorState";
}

template<>
inline const char * name<open_manipulator_msgs::srv::SetActuatorState>()
{
  return "open_manipulator_msgs/srv/SetActuatorState";
}

template<>
struct has_fixed_size<open_manipulator_msgs::srv::SetActuatorState>
  : std::integral_constant<
    bool,
    has_fixed_size<open_manipulator_msgs::srv::SetActuatorState_Request>::value &&
    has_fixed_size<open_manipulator_msgs::srv::SetActuatorState_Response>::value
  >
{
};

template<>
struct has_bounded_size<open_manipulator_msgs::srv::SetActuatorState>
  : std::integral_constant<
    bool,
    has_bounded_size<open_manipulator_msgs::srv::SetActuatorState_Request>::value &&
    has_bounded_size<open_manipulator_msgs::srv::SetActuatorState_Response>::value
  >
{
};

template<>
struct is_service<open_manipulator_msgs::srv::SetActuatorState>
  : std::true_type
{
};

template<>
struct is_service_request<open_manipulator_msgs::srv::SetActuatorState_Request>
  : std::true_type
{
};

template<>
struct is_service_response<open_manipulator_msgs::srv::SetActuatorState_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // OPEN_MANIPULATOR_MSGS__SRV__DETAIL__SET_ACTUATOR_STATE__TRAITS_HPP_
