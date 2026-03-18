// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from go2_interface:msg/CameraGimbalCmd.idl
// generated code does not contain a copyright notice
#include "go2_interface/msg/detail/camera_gimbal_cmd__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
go2_interface__msg__CameraGimbalCmd__init(go2_interface__msg__CameraGimbalCmd * msg)
{
  if (!msg) {
    return false;
  }
  // yaw
  // pitch
  // gimbal_mode
  return true;
}

void
go2_interface__msg__CameraGimbalCmd__fini(go2_interface__msg__CameraGimbalCmd * msg)
{
  if (!msg) {
    return;
  }
  // yaw
  // pitch
  // gimbal_mode
}

bool
go2_interface__msg__CameraGimbalCmd__are_equal(const go2_interface__msg__CameraGimbalCmd * lhs, const go2_interface__msg__CameraGimbalCmd * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // yaw
  if (lhs->yaw != rhs->yaw) {
    return false;
  }
  // pitch
  if (lhs->pitch != rhs->pitch) {
    return false;
  }
  // gimbal_mode
  if (lhs->gimbal_mode != rhs->gimbal_mode) {
    return false;
  }
  return true;
}

bool
go2_interface__msg__CameraGimbalCmd__copy(
  const go2_interface__msg__CameraGimbalCmd * input,
  go2_interface__msg__CameraGimbalCmd * output)
{
  if (!input || !output) {
    return false;
  }
  // yaw
  output->yaw = input->yaw;
  // pitch
  output->pitch = input->pitch;
  // gimbal_mode
  output->gimbal_mode = input->gimbal_mode;
  return true;
}

go2_interface__msg__CameraGimbalCmd *
go2_interface__msg__CameraGimbalCmd__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  go2_interface__msg__CameraGimbalCmd * msg = (go2_interface__msg__CameraGimbalCmd *)allocator.allocate(sizeof(go2_interface__msg__CameraGimbalCmd), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(go2_interface__msg__CameraGimbalCmd));
  bool success = go2_interface__msg__CameraGimbalCmd__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
go2_interface__msg__CameraGimbalCmd__destroy(go2_interface__msg__CameraGimbalCmd * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    go2_interface__msg__CameraGimbalCmd__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
go2_interface__msg__CameraGimbalCmd__Sequence__init(go2_interface__msg__CameraGimbalCmd__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  go2_interface__msg__CameraGimbalCmd * data = NULL;

  if (size) {
    data = (go2_interface__msg__CameraGimbalCmd *)allocator.zero_allocate(size, sizeof(go2_interface__msg__CameraGimbalCmd), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = go2_interface__msg__CameraGimbalCmd__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        go2_interface__msg__CameraGimbalCmd__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
go2_interface__msg__CameraGimbalCmd__Sequence__fini(go2_interface__msg__CameraGimbalCmd__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      go2_interface__msg__CameraGimbalCmd__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

go2_interface__msg__CameraGimbalCmd__Sequence *
go2_interface__msg__CameraGimbalCmd__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  go2_interface__msg__CameraGimbalCmd__Sequence * array = (go2_interface__msg__CameraGimbalCmd__Sequence *)allocator.allocate(sizeof(go2_interface__msg__CameraGimbalCmd__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = go2_interface__msg__CameraGimbalCmd__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
go2_interface__msg__CameraGimbalCmd__Sequence__destroy(go2_interface__msg__CameraGimbalCmd__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    go2_interface__msg__CameraGimbalCmd__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
go2_interface__msg__CameraGimbalCmd__Sequence__are_equal(const go2_interface__msg__CameraGimbalCmd__Sequence * lhs, const go2_interface__msg__CameraGimbalCmd__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!go2_interface__msg__CameraGimbalCmd__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
go2_interface__msg__CameraGimbalCmd__Sequence__copy(
  const go2_interface__msg__CameraGimbalCmd__Sequence * input,
  go2_interface__msg__CameraGimbalCmd__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(go2_interface__msg__CameraGimbalCmd);
    go2_interface__msg__CameraGimbalCmd * data =
      (go2_interface__msg__CameraGimbalCmd *)realloc(output->data, allocation_size);
    if (!data) {
      return false;
    }
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!go2_interface__msg__CameraGimbalCmd__init(&data[i])) {
        /* free currently allocated and return false */
        for (; i-- > output->capacity; ) {
          go2_interface__msg__CameraGimbalCmd__fini(&data[i]);
        }
        free(data);
        return false;
      }
    }
    output->data = data;
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!go2_interface__msg__CameraGimbalCmd__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
