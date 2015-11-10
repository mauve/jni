
#pragma once

#include <jni/raw/types.hpp>

#include <utility>

namespace jni {

template <typename T> class local_ref;
template <typename T> class global_ref;

namespace raw {

inline object_ref extract_reference(const object_ref &value) {
  return value;
}

inline object_ref extract_reference(const array_ref &value) {
  return static_cast<object_ref>(value);
}

inline object_ref extract_reference(const string_ref &value) {
  return static_cast<object_ref>(value);
}

inline object_ref extract_reference(const class_ref &value) {
  return static_cast<object_ref>(value);
}

inline object_ref extract_reference(const throwable_ref &value) {
  return static_cast<object_ref>(value);
}

template <typename T>
inline object_ref extract_reference(const local_ref<T> &value) {
  return extract_reference(value.raw());
}

template <typename T>
inline object_ref extract_reference(const global_ref<T> &value) {
  return extract_reference(value.raw());
}

template <typename ElementType>
inline object_ref extract_reference(const typed_array_ref<ElementType> &value) {
  return extract_reference(static_cast<array_ref>(value));
}

template <typename RefType, typename StateType>
inline object_ref extract_reference(const std::pair<RefType, StateType> &value) {
  return extract_reference(value.first);
}

} // namespace raw
} // namespace jni
