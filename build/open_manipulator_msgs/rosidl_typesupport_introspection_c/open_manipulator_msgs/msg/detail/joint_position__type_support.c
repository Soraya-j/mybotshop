// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from open_manipulator_msgs:msg/JointPosition.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "open_manipulator_msgs/msg/detail/joint_position__rosidl_typesupport_introspection_c.h"
#include "open_manipulator_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "open_manipulator_msgs/msg/detail/joint_position__functions.h"
#include "open_manipulator_msgs/msg/detail/joint_position__struct.h"


// Include directives for member types
// Member `joint_name`
#include "rosidl_runtime_c/string_functions.h"
// Member `position`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void JointPosition__rosidl_typesupport_introspection_c__JointPosition_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  open_manipulator_msgs__msg__JointPosition__init(message_memory);
}

void JointPosition__rosidl_typesupport_introspection_c__JointPosition_fini_function(void * message_memory)
{
  open_manipulator_msgs__msg__JointPosition__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember JointPosition__rosidl_typesupport_introspection_c__JointPosition_message_member_array[4] = {
  {
    "joint_name",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(open_manipulator_msgs__msg__JointPosition, joint_name),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "position",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(open_manipulator_msgs__msg__JointPosition, position),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "max_accelerations_scaling_factor",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(open_manipulator_msgs__msg__JointPosition, max_accelerations_scaling_factor),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "max_velocity_scaling_factor",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(open_manipulator_msgs__msg__JointPosition, max_velocity_scaling_factor),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers JointPosition__rosidl_typesupport_introspection_c__JointPosition_message_members = {
  "open_manipulator_msgs__msg",  // message namespace
  "JointPosition",  // message name
  4,  // number of fields
  sizeof(open_manipulator_msgs__msg__JointPosition),
  JointPosition__rosidl_typesupport_introspection_c__JointPosition_message_member_array,  // message members
  JointPosition__rosidl_typesupport_introspection_c__JointPosition_init_function,  // function to initialize message memory (memory has to be allocated)
  JointPosition__rosidl_typesupport_introspection_c__JointPosition_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t JointPosition__rosidl_typesupport_introspection_c__JointPosition_message_type_support_handle = {
  0,
  &JointPosition__rosidl_typesupport_introspection_c__JointPosition_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_open_manipulator_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, open_manipulator_msgs, msg, JointPosition)() {
  if (!JointPosition__rosidl_typesupport_introspection_c__JointPosition_message_type_support_handle.typesupport_identifier) {
    JointPosition__rosidl_typesupport_introspection_c__JointPosition_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &JointPosition__rosidl_typesupport_introspection_c__JointPosition_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
