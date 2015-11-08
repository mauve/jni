
#pragma once

#ifndef RAW_METHOD_INCLUDING
#error "method.ipp cannot be included directly"
#endif

#include <string>

namespace jni {
namespace raw {

namespace detail {

template <typename R> struct dispatcher {
  static R call(environment &, object_ref instance, method_id mid,
                const value *pack);
};

#define DEFINE_DISPATCHER(type, method)                                        \
  \
template<> inline static type                                                  \
  dispatcher<type>::call(environment &env, object_ref instance, method_id mid, \
                         const value *pack) {                                  \
    return env.method(instance, mid, pack);                                    \
  \
}

DEFINE_DISPATCHER(bool, call_bool_method)
DEFINE_DISPATCHER(std::uint8_t, call_byte_method)
DEFINE_DISPATCHER(std::uint16_t, call_char_method)
DEFINE_DISPATCHER(std::int16_t, call_short_method)
DEFINE_DISPATCHER(std::int32_t, call_int_method)
DEFINE_DISPATCHER(std::int64_t, call_long_method)
DEFINE_DISPATCHER(float, call_float_method)
DEFINE_DISPATCHER(double, call_double_method)
DEFINE_DISPATCHER(void, call_void_method)
DEFINE_DISPATCHER(object_ref, call_object_method)

#undef DEFINE_DISPATCHER

template <>
inline static string_ref
dispatcher<string_ref>::call(environment &env, object_ref instance,
                             method_id mid, const value *pack) {
  return static_cast<string_ref>(env.call_object_method(instance, mid, pack));
}

template <>
inline static class_ref
dispatcher<class_ref>::call(environment &env, object_ref instance,
                            method_id mid, const value *pack) {
  return static_cast<class_ref>(env.call_object_method(instance, mid, pack));
}

template <>
inline static throwable_ref
dispatcher<throwable_ref>::call(environment &env, object_ref instance,
                                method_id mid, const value *pack) {
  return static_cast<throwable_ref>(
      env.call_object_method(instance, mid, pack));
}

} // namespace detail

template <typename R, typename... Args>
inline method<R(Args...)>::method(method_id mid)
    : _mid(mid) {}

template <typename R, typename... Args>
inline method<R(Args...)>::method(environment &env, raw::class_ref cls,
                                  const char *name)
    : _mid{nullptr} {
  signature<R(Args...)> signature;

  _mid = env.get_method_id(cls, name, signature.get());
  if (!_mid) {
    // TODO better error
    throw std::runtime_error(std::string{"method '"} + name +
                             "' with signature '" + signature.get() +
                             "' not found");
  }
}

template <typename R, typename... Args>
R method<R(Args...)>::operator()(environment &env, object_ref instance,
                                 Args... args) {
  std::array<value, sizeof...(args)> pack{to_value(args)...};

  return detail::dispatcher<R>::call(env, instance, _mid, pack.data());
}

} // namespace raw
} // namespace jni
