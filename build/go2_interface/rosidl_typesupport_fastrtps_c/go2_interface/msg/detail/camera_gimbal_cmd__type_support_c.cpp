// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from go2_interface:msg/CameraGimbalCmd.idl
// generated code does not contain a copyright notice
#include "go2_interface/msg/detail/camera_gimbal_cmd__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "go2_interface/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "go2_interface/msg/detail/camera_gimbal_cmd__struct.h"
#include "go2_interface/msg/detail/camera_gimbal_cmd__functions.h"
#include "fastcdr/Cdr.h"

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

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif


// forward declare type support functions


using _CameraGimbalCmd__ros_msg_type = go2_interface__msg__CameraGimbalCmd;

static bool _CameraGimbalCmd__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _CameraGimbalCmd__ros_msg_type * ros_message = static_cast<const _CameraGimbalCmd__ros_msg_type *>(untyped_ros_message);
  // Field name: yaw
  {
    cdr << ros_message->yaw;
  }

  // Field name: pitch
  {
    cdr << ros_message->pitch;
  }

  // Field name: gimbal_mode
  {
    cdr << ros_message->gimbal_mode;
  }

  return true;
}

static bool _CameraGimbalCmd__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _CameraGimbalCmd__ros_msg_type * ros_message = static_cast<_CameraGimbalCmd__ros_msg_type *>(untyped_ros_message);
  // Field name: yaw
  {
    cdr >> ros_message->yaw;
  }

  // Field name: pitch
  {
    cdr >> ros_message->pitch;
  }

  // Field name: gimbal_mode
  {
    cdr >> ros_message->gimbal_mode;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_go2_interface
size_t get_serialized_size_go2_interface__msg__CameraGimbalCmd(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _CameraGimbalCmd__ros_msg_type * ros_message = static_cast<const _CameraGimbalCmd__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name yaw
  {
    size_t item_size = sizeof(ros_message->yaw);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name pitch
  {
    size_t item_size = sizeof(ros_message->pitch);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name gimbal_mode
  {
    size_t item_size = sizeof(ros_message->gimbal_mode);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _CameraGimbalCmd__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_go2_interface__msg__CameraGimbalCmd(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_go2_interface
size_t max_serialized_size_go2_interface__msg__CameraGimbalCmd(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: yaw
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }
  // member: pitch
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }
  // member: gimbal_mode
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint8_t);
  }

  return current_alignment - initial_alignment;
}

static size_t _CameraGimbalCmd__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_go2_interface__msg__CameraGimbalCmd(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_CameraGimbalCmd = {
  "go2_interface::msg",
  "CameraGimbalCmd",
  _CameraGimbalCmd__cdr_serialize,
  _CameraGimbalCmd__cdr_deserialize,
  _CameraGimbalCmd__get_serialized_size,
  _CameraGimbalCmd__max_serialized_size
};

static rosidl_message_type_support_t _CameraGimbalCmd__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_CameraGimbalCmd,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, go2_interface, msg, CameraGimbalCmd)() {
  return &_CameraGimbalCmd__type_support;
}

#if defined(__cplusplus)
}
#endif
