
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
DEFINE_DISPATCHER(local_ref<object_ref>, call_object_method)

#undef DEFINE_DISPATCHER

template <>
inline static local_ref<string_ref>
dispatcher<local_ref<string_ref>>::call(environment &env, object_ref instance,
                                        method_id mid, const value *pack) {
  return local_ref<string_ref>{env.call_object_method(instance, mid, pack)};
}

template <>
inline static local_ref<class_ref>
dispatcher<local_ref<class_ref>>::call(environment &env, object_ref instance,
                                       method_id mid, const value *pack) {
  return local_ref<class_ref>{env.call_object_method(instance, mid, pack)};
}

template <>
inline static local_ref<throwable_ref>
dispatcher<local_ref<throwable_ref>>::call(environment &env,
                                           object_ref instance, method_id mid,
                                           const value *pack) {
  return local_ref<throwable_ref>{env.call_object_method(instance, mid, pack)};
}

} // namespace detail

template <typename R, typename... Args>
inline method<R(Args...)>::method()
    : _mid{nullptr} {}

template <typename R, typename... Args>
inline method<R(Args...)>::method(method_id mid)
    : _mid{mid} {}

template <typename R, typename... Args>
inline method<R(Args...)>::method(environment &env, raw::class_ref cls,
                                  const char *name)
    : _mid{nullptr} {
  signature<add_local_ref_t<R>(add_local_ref_t<Args>...)> signature;

  _mid = env.get_method_id(cls, name, signature.get());
  if (!_mid) {
    // TODO better error
    throw std::runtime_error(std::string{"method '"} + name +
                             "' with signature '" + signature.get() +
                             "' not found");
  }
}

template <typename R, typename... Args>
inline method<R(Args...)>::method(java::lang::Class &cls, const char *name) {
  locateMethod(cls, name);
}

template <typename R, typename... Args>
template <typename... CallingArgs>
add_local_ref_t<R> method<R(Args...)>::
operator()(environment &env, object_ref instance, CallingArgs... args) {
  if (!_mid) {
    throw std::runtime_error("cannot call method with nullptr method_id");
  }

  static_assert(sizeof...(Args) == sizeof...(CallingArgs),
                "CallingArgs and Args need to be the same size");

  std::array<value, sizeof...(args)> pack{to_value(args)...};

  return detail::dispatcher<add_local_ref_t<R>>::call(env, instance, _mid,
                                                      pack.data());
}

template <typename R, typename... Args>
void method<R(Args...)>::locateMethod(java::lang::Class &cls,
                                      const char *name) {
  _mid = cls.getMethod<R(Args...)>(name);
}

} // namespace raw
} // namespace jni
