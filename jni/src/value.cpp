
#include <jni/raw/value.hpp>

namespace jni {
namespace raw {

value to_value(bool v) {
  value r;
  r.z = static_cast<std::uint8_t>(v);
  return r;
}

value to_value(std::int8_t v) {
  value r;
  r.b = v;
  return r;
}

value to_value(std::uint16_t v) {
  value r;
  r.c = v;
  return r;
}

value to_value(std::int16_t v) {
  value r;
  r.s = v;
  return r;
}

value to_value(std::int32_t v) {
  value r;
  r.i = v;
  return r;
}

value to_value(std::int64_t v) {
  value r;
  r.j = v;
  return r;
}

value to_value(float v) {
  value r;
  r.f = v;
  return r;
}

value to_value(double v) {
  value r;
  r.d = v;
  return r;
}

value to_value(object_ref v) {
  value r;
  r.l = v;
  return r;
}

} // namespace raw

} // namespace jni
