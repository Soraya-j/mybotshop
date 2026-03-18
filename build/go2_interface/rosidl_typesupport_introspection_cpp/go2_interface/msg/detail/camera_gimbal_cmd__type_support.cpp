// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from go2_interface:msg/CameraGimbalCmd.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "go2_interface/msg/detail/camera_gimbal_cmd__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace go2_interface
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void CameraGimbalCmd_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) go2_interface::msg::CameraGimbalCmd(_init);
}

void CameraGimbalCmd_fini_function(void * message_memory)
{
  auto typed_message = static_cast<go2_interface::msg::CameraGimbalCmd *>(message_memory);
  typed_message->~CameraGimbalCmd();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CameraGimbalCmd_message_member_array[3] = {
  {
    "yaw",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT64,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(go2_interface::msg::CameraGimbalCmd, yaw),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "pitch",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT64,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(go2_interface::msg::CameraGimbalCmd, pitch),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "gimbal_mode",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(go2_interface::msg::CameraGimbalCmd, gimbal_mode),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CameraGimbalCmd_message_members = {
  "go2_interface::msg",  // message namespace
  "CameraGimbalCmd",  // message name
  3,  // number of fields
  sizeof(go2_interface::msg::CameraGimbalCmd),
  CameraGimbalCmd_message_member_array,  // message members
  CameraGimbalCmd_init_function,  // function to initialize message memory (memory has to be allocated)
  CameraGimbalCmd_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CameraGimbalCmd_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CameraGimbalCmd_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace go2_interface


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<go2_interface::msg::CameraGimbalCmd>()
{
  return &::go2_interface::msg::rosidl_typesupport_introspection_cpp::CameraGimbalCmd_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, go2_interface, msg, CameraGimbalCmd)() {
  return &::go2_interface::msg::rosidl_typesupport_introspection_cpp::CameraGimbalCmd_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
