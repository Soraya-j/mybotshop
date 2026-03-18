// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from go2_interface:msg/CameraGimbalCmd.idl
// generated code does not contain a copyright notice

#ifndef GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__STRUCT_H_
#define GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in msg/CameraGimbalCmd in the package go2_interface.
typedef struct go2_interface__msg__CameraGimbalCmd
{
  int64_t yaw;
  int64_t pitch;
  int8_t gimbal_mode;
} go2_interface__msg__CameraGimbalCmd;

// Struct for a sequence of go2_interface__msg__CameraGimbalCmd.
typedef struct go2_interface__msg__CameraGimbalCmd__Sequence
{
  go2_interface__msg__CameraGimbalCmd * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} go2_interface__msg__CameraGimbalCmd__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__STRUCT_H_
