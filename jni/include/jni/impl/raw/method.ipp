
#pragma once

#ifndef RAW_METHOD_INCLUDING
#error "method.ipp cannot be included directly"
#endif

#include <string>
#include <jni/detail/throw_if_exception.hpp>

namespace jni {
namespace raw {

namespace detail {

template <typename R> struct dispatcher {
  static R call_instance(object_ref instance, method_id mid, const value *pack);
  static R call_static(class_ref cls, method_id mid, const value *pack);
};

#define DEFINE_DISPATCHER(type, method)                                        \
  \
template<> inline static type                                                  \
  dispatcher<type>::call_instance(object_ref instance, method_id mid,          \
                                  const value *pack) {                         \
    auto result = environment::current().method(instance, mid, pack);          \
    throw_if_exception();                                                      \
    return result;                                                             \
  }                                                                            \
  \
template<> inline static type                                                  \
  dispatcher<type>::call_static(class_ref cls, method_id mid,                  \
                                const value *pack) {                           \
    auto result = environment::current().method(cls, mid, pack);               \
    throw_if_exception();                                                      \
    return result;                                                             \
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
DEFINE_DISPATCHER(local_ref<object_ref>, call_object_method)

#undef DEFINE_DISPATCHER

#define DEFINE_OBJECT_DISPATCHER(ref_type)                                     \
  \
template<> inline static local_ref<ref_type>                                   \
  dispatcher<local_ref<ref_type>>::call_instance(                              \
      object_ref instance, method_id mid, const value *pack) {                 \
    auto result =                                                              \
        environment::current().call_object_method(instance, mid, pack);        \
    throw_if_exception();                                                      \
    return ref_cast<ref_type>(std::move(result));                              \
  }                                                                            \
  \
template<> inline static local_ref<ref_type>                                   \
  dispatcher<local_ref<ref_type>>::call_static(class_ref cls, method_id mid,   \
                                               const value *pack) {            \
    auto result = environment::current().call_object_method(cls, mid, pack);   \
    throw_if_exception();                                                      \
    return ref_cast<ref_type>(std::move(result));                              \
  \
}

DEFINE_OBJECT_DISPATCHER(string_ref)
DEFINE_OBJECT_DISPATCHER(class_ref)
DEFINE_OBJECT_DISPATCHER(throwable_ref)
DEFINE_OBJECT_DISPATCHER(typed_array_ref<object_ref>)
DEFINE_OBJECT_DISPATCHER(typed_array_ref<string_ref>)
DEFINE_OBJECT_DISPATCHER(typed_array_ref<class_ref>)
DEFINE_OBJECT_DISPATCHER(typed_array_ref<throwable_ref>)

#undef DEFINE_OBJECT_DISPATCHER

template <>
inline static void dispatcher<void>::call_instance(object_ref instance,
                                                   method_id mid,
                                                   const value *pack) {
  environment::current().call_void_method(instance, mid, pack);
  throw_if_exception();
}

template <>
inline static void dispatcher<void>::call_static(class_ref cls, method_id mid,
                                                 const value *pack) {
  environment::current().call_void_method(cls, mid, pack);
  throw_if_exception();
}

} // namespace detail

//
// method
//

template <typename R, typename... Args>
inline method<R(Args...)>::method(method_id mid)
    : _mid{mid} {
  if (!_mid) {
    throw std::invalid_argument{"cannot construct method from nullptr mid"};
  }
}

template <typename R, typename... Args>
inline method<R(Args...)>::method(java::lang::Class &cls, const char *name)
    : _mid{cls.getMethod<R(Args...)>(name)} {}

template <typename R, typename... Args>
inline method<R(Args...)>::method(java::lang::Class &cls, const char *name,
                                  const char *signature)
    : _mid{cls.getMethod(name, signature)} {}

template <typename R, typename... Args>
template <typename... CallingArgs>
add_local_ref_t<R> method<R(Args...)>::
operator()(const java::lang::Object &instance, CallingArgs... args) {
  static_assert(sizeof...(Args) == sizeof...(CallingArgs),
                "CallingArgs and Args need to be the same size");

  std::array<value, sizeof...(args)> pack{to_value(args)...};

  return detail::dispatcher<add_local_ref_t<R>>::call_instance(
      extract_reference(instance), _mid, pack.data());
}

//
// static_method
//

template <typename R, typename... Args>
inline static_method<R(Args...)>::static_method(method_id mid)
    : _mid{mid} {
  if (!_mid) {
    throw std::invalid_argument{
        "cannot construct static_method from nullptr mid"};
  }
}

template <typename R, typename... Args>
inline static_method<R(Args...)>::static_method(java::lang::Class &cls,
                                                const char *name)
    : _mid{cls.getStaticMethod<R(Args...)>(name)} {}

template <typename R, typename... Args>
inline static_method<R(Args...)>::static_method(java::lang::Class &cls,
                                                const char *name,
                                                const char *signature)
    : _mid{cls.getStaticMethod(name, signature)} {}

template <typename R, typename... Args>
template <typename... CallingArgs>
add_local_ref_t<R> static_method<R(Args...)>::
operator()(const java::lang::Class &cls, CallingArgs... args) {
  static_assert(sizeof...(Args) == sizeof...(CallingArgs),
                "CallingArgs and Args need to be the same size");

  std::array<value, sizeof...(args)> pack{to_value(args)...};

  return detail::dispatcher<add_local_ref_t<R>>::call_static(
      extract_reference(cls), _mid, pack.data());
}

} // namespace raw
} // namespace jni
