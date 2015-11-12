
#pragma once

#include <jni/config.hpp>
#include <jni/raw/types.hpp>

#include <type_traits>

namespace jni {

template <typename T> class local_ref;
template <typename T> class global_ref;

template <typename T> class JNI_EXPORT local_ref {

  static_assert(std::is_convertible<T, raw::object_ref>::value,
                "T must be convertible to jni::raw::object_ref");

public:
  local_ref();
  local_ref(std::nullptr_t);
  explicit local_ref(T ref);
  local_ref(const local_ref &other);
  local_ref(local_ref &&other);
  local_ref(global_ref<T> &&other);
  ~local_ref();

  void release();

  local_ref &operator=(const local_ref &other);
  local_ref &operator=(local_ref &&other);
  local_ref &operator=(const global_ref<T> &other);
  local_ref &operator=(global_ref<T> &&other);

  void assign(T ref);
  T raw() const;
  T leak();

  bool operator!();

private:
  static T new_ref(T ref);

  T _ref;
};

template <typename Target, typename Source>
inline local_ref<Target> ref_cast(local_ref<Source> &&src) {
  return local_ref<Target>{reinterpret_cast<Target>(src.leak())};
}

template <typename T>
inline bool operator==(const local_ref<T> &ref, std::nullptr_t) {
  return ref.raw() == nullptr;
}

template <typename T>
inline bool operator==(std::nullptr_t, const local_ref<T> &ref) {
  return ref.raw() == nullptr;
}

template <typename T>
inline bool operator!=(const local_ref<T> &ref, std::nullptr_t) {
  return ref.raw() != nullptr;
}

template <typename T>
inline bool operator!=(std::nullptr_t, const local_ref<T> &ref) {
  return ref.raw() != nullptr;
}

template <typename T> local_ref<T> make_local_ref(T ref);

extern template class local_ref<raw::object_ref>;
extern template class local_ref<raw::class_ref>;
extern template class local_ref<raw::string_ref>;
extern template class local_ref<raw::throwable_ref>;
extern template class local_ref<raw::typed_array_ref<bool>>;
extern template class local_ref<raw::typed_array_ref<std::uint8_t>>;
extern template class local_ref<raw::typed_array_ref<std::uint16_t>>;
extern template class local_ref<raw::typed_array_ref<std::int16_t>>;
extern template class local_ref<raw::typed_array_ref<std::int32_t>>;
extern template class local_ref<raw::typed_array_ref<std::int64_t>>;
extern template class local_ref<raw::typed_array_ref<float>>;
extern template class local_ref<raw::typed_array_ref<double>>;
extern template class local_ref<raw::typed_array_ref<raw::object_ref>>;
extern template class local_ref<raw::typed_array_ref<raw::class_ref>>;
extern template class local_ref<raw::typed_array_ref<raw::string_ref>>;
extern template class local_ref<raw::typed_array_ref<raw::throwable_ref>>;

extern template local_ref<raw::object_ref> make_local_ref(raw::object_ref ref);
extern template local_ref<raw::class_ref> make_local_ref(raw::class_ref ref);
extern template local_ref<raw::string_ref> make_local_ref(raw::string_ref ref);
extern template local_ref<raw::throwable_ref>
make_local_ref(raw::throwable_ref ref);
extern template local_ref<raw::typed_array_ref<bool>>
make_local_ref(raw::typed_array_ref<bool> ref);
extern template local_ref<raw::typed_array_ref<std::uint8_t>>
make_local_ref(raw::typed_array_ref<std::uint8_t> ref);
extern template local_ref<raw::typed_array_ref<std::uint16_t>>
make_local_ref(raw::typed_array_ref<std::uint16_t> ref);
extern template local_ref<raw::typed_array_ref<std::int16_t>>
make_local_ref(raw::typed_array_ref<std::int16_t> ref);
extern template local_ref<raw::typed_array_ref<std::int32_t>>
make_local_ref(raw::typed_array_ref<std::int32_t> ref);
extern template local_ref<raw::typed_array_ref<std::int64_t>>
make_local_ref(raw::typed_array_ref<std::int64_t> ref);
extern template local_ref<raw::typed_array_ref<float>>
make_local_ref(raw::typed_array_ref<float> ref);
extern template local_ref<raw::typed_array_ref<double>>
make_local_ref(raw::typed_array_ref<double> ref);
extern template local_ref<raw::typed_array_ref<raw::object_ref>>
make_local_ref(raw::typed_array_ref<raw::object_ref> ref);
extern template local_ref<raw::typed_array_ref<raw::class_ref>>
make_local_ref(raw::typed_array_ref<raw::class_ref> ref);
extern template local_ref<raw::typed_array_ref<raw::string_ref>>
make_local_ref(raw::typed_array_ref<raw::string_ref> ref);
extern template local_ref<raw::typed_array_ref<raw::throwable_ref>>
make_local_ref(raw::typed_array_ref<raw::throwable_ref> ref);

} // namespace jni
