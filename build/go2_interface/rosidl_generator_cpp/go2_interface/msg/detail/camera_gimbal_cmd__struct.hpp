// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from go2_interface:msg/CameraGimbalCmd.idl
// generated code does not contain a copyright notice

#ifndef GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__STRUCT_HPP_
#define GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__go2_interface__msg__CameraGimbalCmd __attribute__((deprecated))
#else
# define DEPRECATED__go2_interface__msg__CameraGimbalCmd __declspec(deprecated)
#endif

namespace go2_interface
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct CameraGimbalCmd_
{
  using Type = CameraGimbalCmd_<ContainerAllocator>;

  explicit CameraGimbalCmd_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->yaw = 0ll;
      this->pitch = 0ll;
      this->gimbal_mode = 0;
    }
  }

  explicit CameraGimbalCmd_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->yaw = 0ll;
      this->pitch = 0ll;
      this->gimbal_mode = 0;
    }
  }

  // field types and members
  using _yaw_type =
    int64_t;
  _yaw_type yaw;
  using _pitch_type =
    int64_t;
  _pitch_type pitch;
  using _gimbal_mode_type =
    int8_t;
  _gimbal_mode_type gimbal_mode;

  // setters for named parameter idiom
  Type & set__yaw(
    const int64_t & _arg)
  {
    this->yaw = _arg;
    return *this;
  }
  Type & set__pitch(
    const int64_t & _arg)
  {
    this->pitch = _arg;
    return *this;
  }
  Type & set__gimbal_mode(
    const int8_t & _arg)
  {
    this->gimbal_mode = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    go2_interface::msg::CameraGimbalCmd_<ContainerAllocator> *;
  using ConstRawPtr =
    const go2_interface::msg::CameraGimbalCmd_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<go2_interface::msg::CameraGimbalCmd_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<go2_interface::msg::CameraGimbalCmd_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      go2_interface::msg::CameraGimbalCmd_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<go2_interface::msg::CameraGimbalCmd_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      go2_interface::msg::CameraGimbalCmd_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<go2_interface::msg::CameraGimbalCmd_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<go2_interface::msg::CameraGimbalCmd_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<go2_interface::msg::CameraGimbalCmd_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__go2_interface__msg__CameraGimbalCmd
    std::shared_ptr<go2_interface::msg::CameraGimbalCmd_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__go2_interface__msg__CameraGimbalCmd
    std::shared_ptr<go2_interface::msg::CameraGimbalCmd_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const CameraGimbalCmd_ & other) const
  {
    if (this->yaw != other.yaw) {
      return false;
    }
    if (this->pitch != other.pitch) {
      return false;
    }
    if (this->gimbal_mode != other.gimbal_mode) {
      return false;
    }
    return true;
  }
  bool operator!=(const CameraGimbalCmd_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct CameraGimbalCmd_

// alias to use template instance with default allocator
using CameraGimbalCmd =
  go2_interface::msg::CameraGimbalCmd_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace go2_interface

#endif  // GO2_INTERFACE__MSG__DETAIL__CAMERA_GIMBAL_CMD__STRUCT_HPP_
