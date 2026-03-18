// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from open_manipulator_msgs:msg/JointPosition.idl
// generated code does not contain a copyright notice

#ifndef OPEN_MANIPULATOR_MSGS__MSG__DETAIL__JOINT_POSITION__TRAITS_HPP_
#define OPEN_MANIPULATOR_MSGS__MSG__DETAIL__JOINT_POSITION__TRAITS_HPP_

#include "open_manipulator_msgs/msg/detail/joint_position__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<open_manipulator_msgs::msg::JointPosition>()
{
  return "open_manipulator_msgs::msg::JointPosition";
}

template<>
inline const char * name<open_manipulator_msgs::msg::JointPosition>()
{
  return "open_manipulator_msgs/msg/JointPosition";
}

template<>
struct has_fixed_size<open_manipulator_msgs::msg::JointPosition>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<open_manipulator_msgs::msg::JointPosition>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<open_manipulator_msgs::msg::JointPosition>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // OPEN_MANIPULATOR_MSGS__MSG__DETAIL__JOINT_POSITION__TRAITS_HPP_
