
#pragma once

#ifndef SIGNATURE_INCLUDING
#error "signature.ipp cannot be included directly"
#endif

#include <jni/raw/types.hpp>
#include <jni/detail/remove_ref.hpp>

namespace jni {
namespace raw {

template <typename R, typename... Args>
inline signature<R(Args...)>::signature()
    : _args{get_signature<remove_ref_t<Args>>::get()...} {
  _signature += "(";
  for (auto arg : _args)
    _signature += arg;
  _signature += ")";
  _signature += get_signature<remove_ref_t<R>>::get();
}

template <typename R, typename... Args>
inline const char *signature<R(Args...)>::get() {
  return _signature.c_str();
}

} // namespace raw
} // namespace jni
