// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from open_manipulator_msgs:srv/SetDrawingTrajectory.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "open_manipulator_msgs/srv/detail/set_drawing_trajectory__rosidl_typesupport_introspection_c.h"
#include "open_manipulator_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "open_manipulator_msgs/srv/detail/set_drawing_trajectory__functions.h"
#include "open_manipulator_msgs/srv/detail/set_drawing_trajectory__struct.h"


// Include directives for member types
// Member `end_effector_name`
// Member `drawing_trajectory_name`
#include "rosidl_runtime_c/string_functions.h"
// Member `param`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void SetDrawingTrajectory_Request__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  open_manipulator_msgs__srv__SetDrawingTrajectory_Request__init(message_memory);
}

void SetDrawingTrajectory_Request__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Request_fini_function(void * message_memory)
{
  open_manipulator_msgs__srv__SetDrawingTrajectory_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember SetDrawingTrajectory_Request__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Request_message_member_array[4] = {
  {
    "end_effector_name",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(open_manipulator_msgs__srv__SetDrawingTrajectory_Request, end_effector_name),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "drawing_trajectory_name",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(open_manipulator_msgs__srv__SetDrawingTrajectory_Request, drawing_trajectory_name),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "param",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(open_manipulator_msgs__srv__SetDrawingTrajectory_Request, param),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "path_time",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(open_manipulator_msgs__srv__SetDrawingTrajectory_Request, path_time),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers SetDrawingTrajectory_Request__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Request_message_members = {
  "open_manipulator_msgs__srv",  // message namespace
  "SetDrawingTrajectory_Request",  // message name
  4,  // number of fields
  sizeof(open_manipulator_msgs__srv__SetDrawingTrajectory_Request),
  SetDrawingTrajectory_Request__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Request_message_member_array,  // message members
  SetDrawingTrajectory_Request__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  SetDrawingTrajectory_Request__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t SetDrawingTrajectory_Request__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Request_message_type_support_handle = {
  0,
  &SetDrawingTrajectory_Request__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Request_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_open_manipulator_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, open_manipulator_msgs, srv, SetDrawingTrajectory_Request)() {
  if (!SetDrawingTrajectory_Request__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Request_message_type_support_handle.typesupport_identifier) {
    SetDrawingTrajectory_Request__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &SetDrawingTrajectory_Request__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "open_manipulator_msgs/srv/detail/set_drawing_trajectory__rosidl_typesupport_introspection_c.h"
// already included above
// #include "open_manipulator_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "open_manipulator_msgs/srv/detail/set_drawing_trajectory__functions.h"
// already included above
// #include "open_manipulator_msgs/srv/detail/set_drawing_trajectory__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void SetDrawingTrajectory_Response__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  open_manipulator_msgs__srv__SetDrawingTrajectory_Response__init(message_memory);
}

void SetDrawingTrajectory_Response__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Response_fini_function(void * message_memory)
{
  open_manipulator_msgs__srv__SetDrawingTrajectory_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember SetDrawingTrajectory_Response__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Response_message_member_array[1] = {
  {
    "is_planned",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(open_manipulator_msgs__srv__SetDrawingTrajectory_Response, is_planned),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers SetDrawingTrajectory_Response__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Response_message_members = {
  "open_manipulator_msgs__srv",  // message namespace
  "SetDrawingTrajectory_Response",  // message name
  1,  // number of fields
  sizeof(open_manipulator_msgs__srv__SetDrawingTrajectory_Response),
  SetDrawingTrajectory_Response__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Response_message_member_array,  // message members
  SetDrawingTrajectory_Response__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  SetDrawingTrajectory_Response__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t SetDrawingTrajectory_Response__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Response_message_type_support_handle = {
  0,
  &SetDrawingTrajectory_Response__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Response_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_open_manipulator_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, open_manipulator_msgs, srv, SetDrawingTrajectory_Response)() {
  if (!SetDrawingTrajectory_Response__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Response_message_type_support_handle.typesupport_identifier) {
    SetDrawingTrajectory_Response__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &SetDrawingTrajectory_Response__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "open_manipulator_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "open_manipulator_msgs/srv/detail/set_drawing_trajectory__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers open_manipulator_msgs__srv__detail__set_drawing_trajectory__rosidl_typesupport_introspection_c__SetDrawingTrajectory_service_members = {
  "open_manipulator_msgs__srv",  // service namespace
  "SetDrawingTrajectory",  // service name
  // these two fields are initialized below on the first access
  NULL,  // request message
  // open_manipulator_msgs__srv__detail__set_drawing_trajectory__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Request_message_type_support_handle,
  NULL  // response message
  // open_manipulator_msgs__srv__detail__set_drawing_trajectory__rosidl_typesupport_introspection_c__SetDrawingTrajectory_Response_message_type_support_handle
};

static rosidl_service_type_support_t open_manipulator_msgs__srv__detail__set_drawing_trajectory__rosidl_typesupport_introspection_c__SetDrawingTrajectory_service_type_support_handle = {
  0,
  &open_manipulator_msgs__srv__detail__set_drawing_trajectory__rosidl_typesupport_introspection_c__SetDrawingTrajectory_service_members,
  get_service_typesupport_handle_function,
};

// Forward declaration of request/response type support functions
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, open_manipulator_msgs, srv, SetDrawingTrajectory_Request)();

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, open_manipulator_msgs, srv, SetDrawingTrajectory_Response)();

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_open_manipulator_msgs
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, open_manipulator_msgs, srv, SetDrawingTrajectory)() {
  if (!open_manipulator_msgs__srv__detail__set_drawing_trajectory__rosidl_typesupport_introspection_c__SetDrawingTrajectory_service_type_support_handle.typesupport_identifier) {
    open_manipulator_msgs__srv__detail__set_drawing_trajectory__rosidl_typesupport_introspection_c__SetDrawingTrajectory_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)open_manipulator_msgs__srv__detail__set_drawing_trajectory__rosidl_typesupport_introspection_c__SetDrawingTrajectory_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, open_manipulator_msgs, srv, SetDrawingTrajectory_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, open_manipulator_msgs, srv, SetDrawingTrajectory_Response)()->data;
  }

  return &open_manipulator_msgs__srv__detail__set_drawing_trajectory__rosidl_typesupport_introspection_c__SetDrawingTrajectory_service_type_support_handle;
}
