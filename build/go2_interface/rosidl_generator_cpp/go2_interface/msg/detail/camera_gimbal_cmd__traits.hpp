// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from go2_interface:msg/CameraGimbalCmd.idl
// generated code does not contain a copyright notice

#ifndef GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__TRAITS_HPP_
#define GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__TRAITS_HPP_

#include "go2_interface/msg/detail/camera_gimbal_cmd__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<go2_interface::msg::CameraGimbalCmd>()
{
  return "go2_interface::msg::CameraGimbalCmd";
}

template<>
inline const char * name<go2_interface::msg::CameraGimbalCmd>()
{
  return "go2_interface/msg/CameraGimbalCmd";
}

template<>
struct has_fixed_size<go2_interface::msg::CameraGimbalCmd>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<go2_interface::msg::CameraGimbalCmd>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<go2_interface::msg::CameraGimbalCmd>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__TRAITS_HPP_
