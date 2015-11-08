
#pragma once

#include <array>
#include <string>

namespace jni {
namespace raw {

template <typename Function> class signature;

//
// given a function type consisting only of types allowed
// in "raw" jni functions produce a signature string suitable
// for JNI GetMethodId
//
template <typename R, typename... Args> class signature<R(Args...)> {
public:
  signature();

  const char *get();

private:
  std::array<const char *, sizeof...(Args)> _args;
  std::string _signature;
};

} // namespace raw
} // namespace jni

#define SIGNATURE_INCLUDING
#include <jni/impl/raw/signature.ipp>
#undef SIGNATURE_INCLUDING
