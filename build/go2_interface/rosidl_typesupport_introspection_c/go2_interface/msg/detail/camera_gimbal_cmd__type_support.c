// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from go2_interface:msg/CameraGimbalCmd.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "go2_interface/msg/detail/camera_gimbal_cmd__rosidl_typesupport_introspection_c.h"
#include "go2_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "go2_interface/msg/detail/camera_gimbal_cmd__functions.h"
#include "go2_interface/msg/detail/camera_gimbal_cmd__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void CameraGimbalCmd__rosidl_typesupport_introspection_c__CameraGimbalCmd_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  go2_interface__msg__CameraGimbalCmd__init(message_memory);
}

void CameraGimbalCmd__rosidl_typesupport_introspection_c__CameraGimbalCmd_fini_function(void * message_memory)
{
  go2_interface__msg__CameraGimbalCmd__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember CameraGimbalCmd__rosidl_typesupport_introspection_c__CameraGimbalCmd_message_member_array[3] = {
  {
    "yaw",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT64,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(go2_interface__msg__CameraGimbalCmd, yaw),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "pitch",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT64,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(go2_interface__msg__CameraGimbalCmd, pitch),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "gimbal_mode",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(go2_interface__msg__CameraGimbalCmd, gimbal_mode),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers CameraGimbalCmd__rosidl_typesupport_introspection_c__CameraGimbalCmd_message_members = {
  "go2_interface__msg",  // message namespace
  "CameraGimbalCmd",  // message name
  3,  // number of fields
  sizeof(go2_interface__msg__CameraGimbalCmd),
  CameraGimbalCmd__rosidl_typesupport_introspection_c__CameraGimbalCmd_message_member_array,  // message members
  CameraGimbalCmd__rosidl_typesupport_introspection_c__CameraGimbalCmd_init_function,  // function to initialize message memory (memory has to be allocated)
  CameraGimbalCmd__rosidl_typesupport_introspection_c__CameraGimbalCmd_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t CameraGimbalCmd__rosidl_typesupport_introspection_c__CameraGimbalCmd_message_type_support_handle = {
  0,
  &CameraGimbalCmd__rosidl_typesupport_introspection_c__CameraGimbalCmd_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_go2_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, go2_interface, msg, CameraGimbalCmd)() {
  if (!CameraGimbalCmd__rosidl_typesupport_introspection_c__CameraGimbalCmd_message_type_support_handle.typesupport_identifier) {
    CameraGimbalCmd__rosidl_typesupport_introspection_c__CameraGimbalCmd_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &CameraGimbalCmd__rosidl_typesupport_introspection_c__CameraGimbalCmd_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
