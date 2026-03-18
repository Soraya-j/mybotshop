// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from go2_interface:srv/Go2Volume.idl
// generated code does not contain a copyright notice

#ifndef GO2_INTERFACE__SRV__DETAIL__GO2_VOLUME__STRUCT_H_
#define GO2_INTERFACE__SRV__DETAIL__GO2_VOLUME__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in srv/Go2Volume in the package go2_interface.
typedef struct go2_interface__srv__Go2Volume_Request
{
  int32_t volume_level;
} go2_interface__srv__Go2Volume_Request;

// Struct for a sequence of go2_interface__srv__Go2Volume_Request.
typedef struct go2_interface__srv__Go2Volume_Request__Sequence
{
  go2_interface__srv__Go2Volume_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} go2_interface__srv__Go2Volume_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'reason'
#include "rosidl_runtime_c/string.h"

// Struct defined in srv/Go2Volume in the package go2_interface.
typedef struct go2_interface__srv__Go2Volume_Response
{
  bool success;
  rosidl_runtime_c__String reason;
} go2_interface__srv__Go2Volume_Response;

// Struct for a sequence of go2_interface__srv__Go2Volume_Response.
typedef struct go2_interface__srv__Go2Volume_Response__Sequence
{
  go2_interface__srv__Go2Volume_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} go2_interface__srv__Go2Volume_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // GO2_INTERFACE__SRV__DETAIL__GO2_VOLUME__STRUCT_H_
