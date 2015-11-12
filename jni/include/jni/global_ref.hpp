
#pragma once

#include <jni/config.hpp>
#include <jni/raw/types.hpp>

#include <type_traits>

namespace jni {

template <typename T> class JNI_EXPORT global_ref {

  static_assert(std::is_convertible<T, raw::object_ref>::value,
                "T must be convertible to jni::raw::object_ref");

public:
  enum steal_t { adopt_existing_global_ref };

  global_ref();
  global_ref(std::nullptr_t);
  explicit global_ref(T ref);
  global_ref(T ref, steal_t);
  global_ref(const global_ref &other);
  global_ref(global_ref &&other);

  template <typename U> explicit global_ref(local_ref<U> &&other);

  ~global_ref();

  void release();

  global_ref &operator=(const global_ref &other);
  global_ref &operator=(global_ref &&other);
  global_ref &operator=(const local_ref<T> &other);
  global_ref &operator=(local_ref<T> &&other);

  bool operator!();

  void assign(T ref);
  T raw() const;
  T leak();

private:
  static T new_ref(T ref);

  T _ref;
};

template <typename Target, typename Source>
inline global_ref<Target> ref_cast(global_ref<Source> &&src) {
  return global_ref<Target>{reinterpret_cast<Target>(src.leak()),
                            global_ref<Target>::adopt_existing_global_ref};
}

template <typename T>
bool operator==(const global_ref<T> &ref, std::nullptr_t);

template <typename T>
bool operator==(std::nullptr_t, const global_ref<T> &ref);

template <typename T>
bool operator!=(const global_ref<T> &ref, std::nullptr_t);

template <typename T>
bool operator!=(std::nullptr_t, const global_ref<T> &ref);

extern template class global_ref<raw::object_ref>;
extern template class global_ref<raw::class_ref>;
extern template class global_ref<raw::string_ref>;
extern template class global_ref<raw::throwable_ref>;
extern template class global_ref<raw::typed_array_ref<bool>>;
extern template class global_ref<raw::typed_array_ref<std::uint8_t>>;
extern template class global_ref<raw::typed_array_ref<std::uint16_t>>;
extern template class global_ref<raw::typed_array_ref<std::int16_t>>;
extern template class global_ref<raw::typed_array_ref<std::int32_t>>;
extern template class global_ref<raw::typed_array_ref<std::int64_t>>;
extern template class global_ref<raw::typed_array_ref<float>>;
extern template class global_ref<raw::typed_array_ref<double>>;
extern template class global_ref<raw::typed_array_ref<raw::object_ref>>;
extern template class global_ref<raw::typed_array_ref<raw::class_ref>>;
extern template class global_ref<raw::typed_array_ref<raw::string_ref>>;
extern template class global_ref<raw::typed_array_ref<raw::throwable_ref>>;

extern template global_ref<raw::object_ref>::global_ref(local_ref<raw::object_ref> &&ref);
extern template global_ref<raw::class_ref>::global_ref(local_ref<raw::class_ref> &&ref);
extern template global_ref<raw::string_ref>::global_ref(local_ref<raw::string_ref> &&ref);
extern template global_ref<raw::throwable_ref>::global_ref(local_ref<raw::throwable_ref> &&ref);
extern template global_ref<raw::typed_array_ref<bool>>::global_ref(local_ref<raw::typed_array_ref<bool>> &&ref);
extern template global_ref<raw::typed_array_ref<std::uint8_t>>::global_ref(local_ref<raw::typed_array_ref<std::uint8_t>> &&ref);
extern template global_ref<raw::typed_array_ref<std::uint16_t>>::global_ref(local_ref<raw::typed_array_ref<std::uint16_t>> &&ref);
extern template global_ref<raw::typed_array_ref<std::int16_t>>::global_ref(local_ref<raw::typed_array_ref<std::int16_t>> &&ref);
extern template global_ref<raw::typed_array_ref<std::int32_t>>::global_ref(local_ref<raw::typed_array_ref<std::int32_t>> &&ref);
extern template global_ref<raw::typed_array_ref<std::int64_t>>::global_ref(local_ref<raw::typed_array_ref<std::int64_t>> &&ref);
extern template global_ref<raw::typed_array_ref<float>>::global_ref(local_ref<raw::typed_array_ref<float>> &&ref);
extern template global_ref<raw::typed_array_ref<double>>::global_ref(local_ref<raw::typed_array_ref<double>> &&ref);
extern template global_ref<raw::typed_array_ref<raw::object_ref>>::global_ref(local_ref<raw::typed_array_ref<raw::object_ref>> &&ref);
extern template global_ref<raw::typed_array_ref<raw::class_ref>>::global_ref(local_ref<raw::typed_array_ref<raw::class_ref>> &&ref);
extern template global_ref<raw::typed_array_ref<raw::string_ref>>::global_ref(local_ref<raw::typed_array_ref<raw::string_ref>> &&ref);
extern template global_ref<raw::typed_array_ref<raw::throwable_ref>>::global_ref(local_ref<raw::typed_array_ref<raw::throwable_ref>> &&ref);

} // namespace jni
