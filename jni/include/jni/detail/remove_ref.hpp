
#pragma once

namespace jni {

// forward decl
template <typename T> class local_ref;
template <typename T> class global_ref;

template <typename T> struct remove_ref { using value_type = T; };

template <typename T> struct remove_ref<local_ref<T>> {
  using value_type = T;
};

template <typename T> struct remove_ref<global_ref<T>> {
  using value_type = T;
};

template <typename T>
using remove_ref_t = typename remove_ref<T>::value_type;

} // namespace jni
