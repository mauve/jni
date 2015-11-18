
#pragma once

#include <jni/environment.hpp>
#include <jni/raw/types.hpp>
#include <jni/raw/value.hpp>
#include <jni/raw/signature.hpp>
#include <jni/detail/remove_ref.hpp>
#include <jni/detail/add_local_ref.hpp>

#include <array>

namespace jni {

namespace java {
namespace lang {
class Class;
class Object;
} // namespace lang
} // namespace java

namespace raw {

template <typename Function> class method;

template <typename R, typename... Args> class method<R(Args...)> {
public:
  method() = delete;
  method(const method &) = default;
  method(method &&) = default;

  method(method_id mid);
  method(java::lang::Class &cls, const char *name);
  method(java::lang::Class &cls, const char *name, const char *signature);

  template <typename... CallingArgs>
  add_local_ref_t<R> operator()(const java::lang::Object &instance,
                                CallingArgs... args);

  method &operator=(const method &) = default;
  method &operator=(method &&) = default;

private:
  method_id _mid;
};

} // namespace raw
} // namespace jni

#define RAW_METHOD_INCLUDING
#include <jni/impl/raw/method.ipp>
#undef RAW_METHOD_INCLUDING
