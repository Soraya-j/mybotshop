// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from go2_interface:srv/Go2Modes.idl
// generated code does not contain a copyright notice

#ifndef GO2_INTERFACE__SRV__DETAIL__GO2_MODES__TRAITS_HPP_
#define GO2_INTERFACE__SRV__DETAIL__GO2_MODES__TRAITS_HPP_

#include "go2_interface/srv/detail/go2_modes__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<go2_interface::srv::Go2Modes_Request>()
{
  return "go2_interface::srv::Go2Modes_Request";
}

template<>
inline const char * name<go2_interface::srv::Go2Modes_Request>()
{
  return "go2_interface/srv/Go2Modes_Request";
}

template<>
struct has_fixed_size<go2_interface::srv::Go2Modes_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<go2_interface::srv::Go2Modes_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<go2_interface::srv::Go2Modes_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<go2_interface::srv::Go2Modes_Response>()
{
  return "go2_interface::srv::Go2Modes_Response";
}

template<>
inline const char * name<go2_interface::srv::Go2Modes_Response>()
{
  return "go2_interface/srv/Go2Modes_Response";
}

template<>
struct has_fixed_size<go2_interface::srv::Go2Modes_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<go2_interface::srv::Go2Modes_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<go2_interface::srv::Go2Modes_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<go2_interface::srv::Go2Modes>()
{
  return "go2_interface::srv::Go2Modes";
}

template<>
inline const char * name<go2_interface::srv::Go2Modes>()
{
  return "go2_interface/srv/Go2Modes";
}

template<>
struct has_fixed_size<go2_interface::srv::Go2Modes>
  : std::integral_constant<
    bool,
    has_fixed_size<go2_interface::srv::Go2Modes_Request>::value &&
    has_fixed_size<go2_interface::srv::Go2Modes_Response>::value
  >
{
};

template<>
struct has_bounded_size<go2_interface::srv::Go2Modes>
  : std::integral_constant<
    bool,
    has_bounded_size<go2_interface::srv::Go2Modes_Request>::value &&
    has_bounded_size<go2_interface::srv::Go2Modes_Response>::value
  >
{
};

template<>
struct is_service<go2_interface::srv::Go2Modes>
  : std::true_type
{
};

template<>
struct is_service_request<go2_interface::srv::Go2Modes_Request>
  : std::true_type
{
};

template<>
struct is_service_response<go2_interface::srv::Go2Modes_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // GO2_INTERFACE__SRV__DETAIL__GO2_MODES__TRAITS_HPP_
