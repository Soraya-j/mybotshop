// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from go2_interface:msg/CameraGimbalCmd.idl
// generated code does not contain a copyright notice

#ifndef GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "go2_interface/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "go2_interface/msg/detail/camera_gimbal_cmd__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace go2_interface
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_go2_interface
cdr_serialize(
  const go2_interface::msg::CameraGimbalCmd & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_go2_interface
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  go2_interface::msg::CameraGimbalCmd & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_go2_interface
get_serialized_size(
  const go2_interface::msg::CameraGimbalCmd & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_go2_interface
max_serialized_size_CameraGimbalCmd(
  bool & full_bounded,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace go2_interface

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_go2_interface
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, go2_interface, msg, CameraGimbalCmd)();

#ifdef __cplusplus
}
#endif

#endif  // GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
