
#pragma once

#ifndef REF_HOLDER_INCLUDING
#error "ref_holder.ipp cannot be included directly"
#endif

#include <utility>

namespace jni {
namespace raw {

template <typename T> inline ref_holder<T>::ref_holder(std::nullptr_t) {}

template <typename T>
inline ref_holder<T>::ref_holder(global_ref<T> &&ref)
    : _ref(std::move(ref)) {}
template <typename T>
inline ref_holder<T>::ref_holder(local_ref<T> &&ref)
    : _ref(std::move(ref)) {}

template <typename T> inline T ref_holder<T>::get_ref() const {
  return _ref.raw();
}

template <typename T> inline T get_ref(const ref_holder<T> &holder) {
  return holder.get_ref();
}

template <typename T> inline T get_ref(const ref_holder<T> *holder) {
  return !holder ? nullptr : holder->get_ref();
}

} // namespace raw

} // namespace jni
