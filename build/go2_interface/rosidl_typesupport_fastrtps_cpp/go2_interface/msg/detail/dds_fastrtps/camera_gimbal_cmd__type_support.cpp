// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__type_support.cpp.em
// with input from go2_interface:msg/CameraGimbalCmd.idl
// generated code does not contain a copyright notice
#include "go2_interface/msg/detail/camera_gimbal_cmd__rosidl_typesupport_fastrtps_cpp.hpp"
#include "go2_interface/msg/detail/camera_gimbal_cmd__struct.hpp"

#include <limits>
#include <stdexcept>
#include <string>
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_fastrtps_cpp/identifier.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_fastrtps_cpp/wstring_conversion.hpp"
#include "fastcdr/Cdr.h"


// forward declaration of message dependencies and their conversion functions

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
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: yaw
  cdr << ros_message.yaw;
  // Member: pitch
  cdr << ros_message.pitch;
  // Member: gimbal_mode
  cdr << ros_message.gimbal_mode;
  return true;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_go2_interface
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  go2_interface::msg::CameraGimbalCmd & ros_message)
{
  // Member: yaw
  cdr >> ros_message.yaw;

  // Member: pitch
  cdr >> ros_message.pitch;

  // Member: gimbal_mode
  cdr >> ros_message.gimbal_mode;

  return true;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_go2_interface
get_serialized_size(
  const go2_interface::msg::CameraGimbalCmd & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: yaw
  {
    size_t item_size = sizeof(ros_message.yaw);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: pitch
  {
    size_t item_size = sizeof(ros_message.pitch);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: gimbal_mode
  {
    size_t item_size = sizeof(ros_message.gimbal_mode);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_go2_interface
max_serialized_size_CameraGimbalCmd(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;


  // Member: yaw
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Member: pitch
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Member: gimbal_mode
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint8_t);
  }

  return current_alignment - initial_alignment;
}

static bool _CameraGimbalCmd__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  auto typed_message =
    static_cast<const go2_interface::msg::CameraGimbalCmd *>(
    untyped_ros_message);
  return cdr_serialize(*typed_message, cdr);
}

static bool _CameraGimbalCmd__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  auto typed_message =
    static_cast<go2_interface::msg::CameraGimbalCmd *>(
    untyped_ros_message);
  return cdr_deserialize(cdr, *typed_message);
}

static uint32_t _CameraGimbalCmd__get_serialized_size(
  const void * untyped_ros_message)
{
  auto typed_message =
    static_cast<const go2_interface::msg::CameraGimbalCmd *>(
    untyped_ros_message);
  return static_cast<uint32_t>(get_serialized_size(*typed_message, 0));
}

static size_t _CameraGimbalCmd__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_CameraGimbalCmd(full_bounded, 0);
}

static message_type_support_callbacks_t _CameraGimbalCmd__callbacks = {
  "go2_interface::msg",
  "CameraGimbalCmd",
  _CameraGimbalCmd__cdr_serialize,
  _CameraGimbalCmd__cdr_deserialize,
  _CameraGimbalCmd__get_serialized_size,
  _CameraGimbalCmd__max_serialized_size
};

static rosidl_message_type_support_t _CameraGimbalCmd__handle = {
  rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
  &_CameraGimbalCmd__callbacks,
  get_message_typesupport_handle_function,
};

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace go2_interface

namespace rosidl_typesupport_fastrtps_cpp
{

template<>
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_EXPORT_go2_interface
const rosidl_message_type_support_t *
get_message_type_support_handle<go2_interface::msg::CameraGimbalCmd>()
{
  return &go2_interface::msg::typesupport_fastrtps_cpp::_CameraGimbalCmd__handle;
}

}  // namespace rosidl_typesupport_fastrtps_cpp

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, go2_interface, msg, CameraGimbalCmd)() {
  return &go2_interface::msg::typesupport_fastrtps_cpp::_CameraGimbalCmd__handle;
}

#ifdef __cplusplus
}
#endif
