#pragma once

#include <cstdint>

namespace jni {

template <typename T> class local_ref;

template <typename T> struct add_local_ref { using value_type = local_ref<T>; };

template <typename T> struct add_local_ref<local_ref<T>> {
  using value_type = local_ref<T>;
};

template <> struct add_local_ref<bool> { using value_type = bool; };

template <> struct add_local_ref<std::uint8_t> {
  using value_type = std::uint8_t;
};

template <> struct add_local_ref<std::uint16_t> {
  using value_type = std::uint16_t;
};

template <> struct add_local_ref<std::int16_t> {
  using value_type = std::int16_t;
};

template <> struct add_local_ref<std::int32_t> {
  using value_type = std::int32_t;
};

template <> struct add_local_ref<std::int64_t> {
  using value_type = std::int64_t;
};

template <> struct add_local_ref<float> { using value_type = float; };

template <> struct add_local_ref<double> { using value_type = double; };

template <> struct add_local_ref<void> { using value_type = void; };

template <typename T>
using add_local_ref_t = typename add_local_ref<T>::value_type;

} // namespace jni
