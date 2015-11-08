
#pragma once

#include <jni/environment.hpp>
#include <jni/raw/types.hpp>
#include <jni/raw/value.hpp>
#include <jni/raw/signature.hpp>

#include <array>

namespace jni {
namespace raw {

template <typename Function> class method;

template <typename R, typename... Args> class method<R(Args...)> {
public:
  method(method_id mid);
  method(environment &env, raw::class_ref cls, const char *name);

  R operator()(environment &env, object_ref instance, Args... args);

private:
  method_id _mid;
};

} // namespace raw
} // namespace jni

#define RAW_METHOD_INCLUDING
#include <jni/impl/raw/method.ipp>
#undef RAW_METHOD_INCLUDING
