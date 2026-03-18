// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from open_manipulator_msgs:msg/KinematicsPose.idl
// generated code does not contain a copyright notice

#ifndef OPEN_MANIPULATOR_MSGS__MSG__DETAIL__KINEMATICS_POSE__TRAITS_HPP_
#define OPEN_MANIPULATOR_MSGS__MSG__DETAIL__KINEMATICS_POSE__TRAITS_HPP_

#include "open_manipulator_msgs/msg/detail/kinematics_pose__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

// Include directives for member types
// Member 'pose'
#include "geometry_msgs/msg/detail/pose__traits.hpp"

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<open_manipulator_msgs::msg::KinematicsPose>()
{
  return "open_manipulator_msgs::msg::KinematicsPose";
}

template<>
inline const char * name<open_manipulator_msgs::msg::KinematicsPose>()
{
  return "open_manipulator_msgs/msg/KinematicsPose";
}

template<>
struct has_fixed_size<open_manipulator_msgs::msg::KinematicsPose>
  : std::integral_constant<bool, has_fixed_size<geometry_msgs::msg::Pose>::value> {};

template<>
struct has_bounded_size<open_manipulator_msgs::msg::KinematicsPose>
  : std::integral_constant<bool, has_bounded_size<geometry_msgs::msg::Pose>::value> {};

template<>
struct is_message<open_manipulator_msgs::msg::KinematicsPose>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // OPEN_MANIPULATOR_MSGS__MSG__DETAIL__KINEMATICS_POSE__TRAITS_HPP_
