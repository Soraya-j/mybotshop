// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from open_manipulator_msgs:srv/GetKinematicsPose.idl
// generated code does not contain a copyright notice
#include "open_manipulator_msgs/srv/detail/get_kinematics_pose__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "open_manipulator_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "open_manipulator_msgs/srv/detail/get_kinematics_pose__struct.h"
#include "open_manipulator_msgs/srv/detail/get_kinematics_pose__functions.h"
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

#include "rosidl_runtime_c/string.h"  // end_effector_name, planning_group
#include "rosidl_runtime_c/string_functions.h"  // end_effector_name, planning_group

// forward declare type support functions


using _GetKinematicsPose_Request__ros_msg_type = open_manipulator_msgs__srv__GetKinematicsPose_Request;

static bool _GetKinematicsPose_Request__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _GetKinematicsPose_Request__ros_msg_type * ros_message = static_cast<const _GetKinematicsPose_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: planning_group
  {
    const rosidl_runtime_c__String * str = &ros_message->planning_group;
    if (str->capacity == 0 || str->capacity <= str->size) {
      fprintf(stderr, "string capacity not greater than size\n");
      return false;
    }
    if (str->data[str->size] != '\0') {
      fprintf(stderr, "string not null-terminated\n");
      return false;
    }
    cdr << str->data;
  }

  // Field name: end_effector_name
  {
    const rosidl_runtime_c__String * str = &ros_message->end_effector_name;
    if (str->capacity == 0 || str->capacity <= str->size) {
      fprintf(stderr, "string capacity not greater than size\n");
      return false;
    }
    if (str->data[str->size] != '\0') {
      fprintf(stderr, "string not null-terminated\n");
      return false;
    }
    cdr << str->data;
  }

  return true;
}

static bool _GetKinematicsPose_Request__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _GetKinematicsPose_Request__ros_msg_type * ros_message = static_cast<_GetKinematicsPose_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: planning_group
  {
    std::string tmp;
    cdr >> tmp;
    if (!ros_message->planning_group.data) {
      rosidl_runtime_c__String__init(&ros_message->planning_group);
    }
    bool succeeded = rosidl_runtime_c__String__assign(
      &ros_message->planning_group,
      tmp.c_str());
    if (!succeeded) {
      fprintf(stderr, "failed to assign string into field 'planning_group'\n");
      return false;
    }
  }

  // Field name: end_effector_name
  {
    std::string tmp;
    cdr >> tmp;
    if (!ros_message->end_effector_name.data) {
      rosidl_runtime_c__String__init(&ros_message->end_effector_name);
    }
    bool succeeded = rosidl_runtime_c__String__assign(
      &ros_message->end_effector_name,
      tmp.c_str());
    if (!succeeded) {
      fprintf(stderr, "failed to assign string into field 'end_effector_name'\n");
      return false;
    }
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_open_manipulator_msgs
size_t get_serialized_size_open_manipulator_msgs__srv__GetKinematicsPose_Request(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _GetKinematicsPose_Request__ros_msg_type * ros_message = static_cast<const _GetKinematicsPose_Request__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name planning_group
  current_alignment += padding +
    eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
    (ros_message->planning_group.size + 1);
  // field.name end_effector_name
  current_alignment += padding +
    eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
    (ros_message->end_effector_name.size + 1);

  return current_alignment - initial_alignment;
}

static uint32_t _GetKinematicsPose_Request__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_open_manipulator_msgs__srv__GetKinematicsPose_Request(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_open_manipulator_msgs
size_t max_serialized_size_open_manipulator_msgs__srv__GetKinematicsPose_Request(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: planning_group
  {
    size_t array_size = 1;

    full_bounded = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }
  // member: end_effector_name
  {
    size_t array_size = 1;

    full_bounded = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }

  return current_alignment - initial_alignment;
}

static size_t _GetKinematicsPose_Request__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_open_manipulator_msgs__srv__GetKinematicsPose_Request(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_GetKinematicsPose_Request = {
  "open_manipulator_msgs::srv",
  "GetKinematicsPose_Request",
  _GetKinematicsPose_Request__cdr_serialize,
  _GetKinematicsPose_Request__cdr_deserialize,
  _GetKinematicsPose_Request__get_serialized_size,
  _GetKinematicsPose_Request__max_serialized_size
};

static rosidl_message_type_support_t _GetKinematicsPose_Request__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_GetKinematicsPose_Request,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, open_manipulator_msgs, srv, GetKinematicsPose_Request)() {
  return &_GetKinematicsPose_Request__type_support;
}

#if defined(__cplusplus)
}
#endif

// already included above
// #include <cassert>
// already included above
// #include <limits>
// already included above
// #include <string>
// already included above
// #include "rosidl_typesupport_fastrtps_c/identifier.h"
// already included above
// #include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
// already included above
// #include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
// already included above
// #include "open_manipulator_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
// already included above
// #include "open_manipulator_msgs/srv/detail/get_kinematics_pose__struct.h"
// already included above
// #include "open_manipulator_msgs/srv/detail/get_kinematics_pose__functions.h"
// already included above
// #include "fastcdr/Cdr.h"

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

#include "open_manipulator_msgs/msg/detail/kinematics_pose__functions.h"  // kinematics_pose
#include "std_msgs/msg/detail/header__functions.h"  // header

// forward declare type support functions
size_t get_serialized_size_open_manipulator_msgs__msg__KinematicsPose(
  const void * untyped_ros_message,
  size_t current_alignment);

size_t max_serialized_size_open_manipulator_msgs__msg__KinematicsPose(
  bool & full_bounded,
  size_t current_alignment);

const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, open_manipulator_msgs, msg, KinematicsPose)();
ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_open_manipulator_msgs
size_t get_serialized_size_std_msgs__msg__Header(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_open_manipulator_msgs
size_t max_serialized_size_std_msgs__msg__Header(
  bool & full_bounded,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_open_manipulator_msgs
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, std_msgs, msg, Header)();


using _GetKinematicsPose_Response__ros_msg_type = open_manipulator_msgs__srv__GetKinematicsPose_Response;

static bool _GetKinematicsPose_Response__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _GetKinematicsPose_Response__ros_msg_type * ros_message = static_cast<const _GetKinematicsPose_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: header
  {
    const message_type_support_callbacks_t * callbacks =
      static_cast<const message_type_support_callbacks_t *>(
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(
        rosidl_typesupport_fastrtps_c, std_msgs, msg, Header
      )()->data);
    if (!callbacks->cdr_serialize(
        &ros_message->header, cdr))
    {
      return false;
    }
  }

  // Field name: kinematics_pose
  {
    const message_type_support_callbacks_t * callbacks =
      static_cast<const message_type_support_callbacks_t *>(
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(
        rosidl_typesupport_fastrtps_c, open_manipulator_msgs, msg, KinematicsPose
      )()->data);
    if (!callbacks->cdr_serialize(
        &ros_message->kinematics_pose, cdr))
    {
      return false;
    }
  }

  return true;
}

static bool _GetKinematicsPose_Response__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _GetKinematicsPose_Response__ros_msg_type * ros_message = static_cast<_GetKinematicsPose_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: header
  {
    const message_type_support_callbacks_t * callbacks =
      static_cast<const message_type_support_callbacks_t *>(
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(
        rosidl_typesupport_fastrtps_c, std_msgs, msg, Header
      )()->data);
    if (!callbacks->cdr_deserialize(
        cdr, &ros_message->header))
    {
      return false;
    }
  }

  // Field name: kinematics_pose
  {
    const message_type_support_callbacks_t * callbacks =
      static_cast<const message_type_support_callbacks_t *>(
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(
        rosidl_typesupport_fastrtps_c, open_manipulator_msgs, msg, KinematicsPose
      )()->data);
    if (!callbacks->cdr_deserialize(
        cdr, &ros_message->kinematics_pose))
    {
      return false;
    }
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_open_manipulator_msgs
size_t get_serialized_size_open_manipulator_msgs__srv__GetKinematicsPose_Response(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _GetKinematicsPose_Response__ros_msg_type * ros_message = static_cast<const _GetKinematicsPose_Response__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name header

  current_alignment += get_serialized_size_std_msgs__msg__Header(
    &(ros_message->header), current_alignment);
  // field.name kinematics_pose

  current_alignment += get_serialized_size_open_manipulator_msgs__msg__KinematicsPose(
    &(ros_message->kinematics_pose), current_alignment);

  return current_alignment - initial_alignment;
}

static uint32_t _GetKinematicsPose_Response__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_open_manipulator_msgs__srv__GetKinematicsPose_Response(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_open_manipulator_msgs
size_t max_serialized_size_open_manipulator_msgs__srv__GetKinematicsPose_Response(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: header
  {
    size_t array_size = 1;


    for (size_t index = 0; index < array_size; ++index) {
      current_alignment +=
        max_serialized_size_std_msgs__msg__Header(
        full_bounded, current_alignment);
    }
  }
  // member: kinematics_pose
  {
    size_t array_size = 1;


    for (size_t index = 0; index < array_size; ++index) {
      current_alignment +=
        max_serialized_size_open_manipulator_msgs__msg__KinematicsPose(
        full_bounded, current_alignment);
    }
  }

  return current_alignment - initial_alignment;
}

static size_t _GetKinematicsPose_Response__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_open_manipulator_msgs__srv__GetKinematicsPose_Response(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_GetKinematicsPose_Response = {
  "open_manipulator_msgs::srv",
  "GetKinematicsPose_Response",
  _GetKinematicsPose_Response__cdr_serialize,
  _GetKinematicsPose_Response__cdr_deserialize,
  _GetKinematicsPose_Response__get_serialized_size,
  _GetKinematicsPose_Response__max_serialized_size
};

static rosidl_message_type_support_t _GetKinematicsPose_Response__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_GetKinematicsPose_Response,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, open_manipulator_msgs, srv, GetKinematicsPose_Response)() {
  return &_GetKinematicsPose_Response__type_support;
}

#if defined(__cplusplus)
}
#endif

#include "rosidl_typesupport_fastrtps_cpp/service_type_support.h"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_fastrtps_c/identifier.h"
// already included above
// #include "open_manipulator_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "open_manipulator_msgs/srv/get_kinematics_pose.h"

#if defined(__cplusplus)
extern "C"
{
#endif

static service_type_support_callbacks_t GetKinematicsPose__callbacks = {
  "open_manipulator_msgs::srv",
  "GetKinematicsPose",
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, open_manipulator_msgs, srv, GetKinematicsPose_Request)(),
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, open_manipulator_msgs, srv, GetKinematicsPose_Response)(),
};

static rosidl_service_type_support_t GetKinematicsPose__handle = {
  rosidl_typesupport_fastrtps_c__identifier,
  &GetKinematicsPose__callbacks,
  get_service_typesupport_handle_function,
};

const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, open_manipulator_msgs, srv, GetKinematicsPose)() {
  return &GetKinematicsPose__handle;
}

#if defined(__cplusplus)
}
#endif
