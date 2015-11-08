
#pragma once

#ifndef RAW_FIELD_INCLUDING
#error "field.ipp cannot be included directly"
#endif

#include <jni/environment.hpp>

namespace jni {
namespace raw {

template <typename T> field<T>::field(field_id fid) : _fid{fid} {}

template <typename T>
T field<T>::read(environment &env, object_ref instance) const {
  return {};
}

template <typename T>
void field<T>::write(environment &env, object_ref instance, const T &value) {}

} // namespace raw
} // namespace jni
