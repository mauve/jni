
#pragma once

#include <boost/system/error_code.hpp>

namespace jni {

enum class error_codes {
  success = 0,
  // map to jni error codes
  unknown = -1,
  thread_detached = -2,
  invalid_version = -3,
  out_of_memory = -4,
  vm_already_created = -5,
  invalid_argument = -6,

  // custom error codes
  vm_destroyed = 1
};

boost::system::error_category &get_error_category();

boost::system::error_code make_error_code(error_codes jni_error);

} // namespace jni

namespace boost {
namespace system {

template <> struct is_error_code_enum<jni::error_codes> {
  static const bool value = true;
};

} // namespace system
} // namespace boost
