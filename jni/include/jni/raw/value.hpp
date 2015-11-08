
#pragma once

#include <cstdint>

#include <jni/config.hpp>
#include <jni/raw/types.hpp>

namespace jni {
namespace raw {
namespace detail {

union _jni_value {
  std::uint8_t z;
  std::int8_t b;
  std::uint16_t c;
  std::int16_t s;
  std::int32_t i;
  std::int64_t j;
  float f;
  double d;
  object_ref l;
};

} // namespace

using value = detail::_jni_value;

JNI_EXPORT value to_value(bool v);
JNI_EXPORT value to_value(std::int8_t v);
JNI_EXPORT value to_value(std::uint16_t v);
JNI_EXPORT value to_value(std::int16_t v);
JNI_EXPORT value to_value(std::int32_t v);
JNI_EXPORT value to_value(std::int64_t v);
JNI_EXPORT value to_value(float v);
JNI_EXPORT value to_value(double v);
JNI_EXPORT value to_value(object_ref v);

} // namespace raw
} // namespace jni
