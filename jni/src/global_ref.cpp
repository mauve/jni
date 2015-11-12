
#include <jni/global_ref.hpp>

#include <jni/local_ref.hpp>
#include <jni/environment.hpp>

namespace jni {

template <typename T> inline global_ref<T>::global_ref() : _ref(nullptr) {}

template <typename T>
inline global_ref<T>::global_ref(std::nullptr_t)
    : _ref(nullptr) {}

template <typename T>
inline global_ref<T>::global_ref(T ref)
    : _ref(new_ref(ref)) {}

template <typename T>
inline global_ref<T>::global_ref(T ref, steal_t)
    : _ref(ref) {}

template <typename T>
inline global_ref<T>::global_ref(const global_ref<T> &other)
    : _ref(new_ref(other._ref)) {}

template <typename T>
inline global_ref<T>::global_ref(global_ref &&other)
    : _ref(static_cast<T>(other._ref)) {
  other._ref = nullptr;
}

template <typename T>
template <typename U>
inline global_ref<T>::global_ref(local_ref<U> &&other)
    : _ref(new_ref(reinterpret_cast<T>(other.raw()))) {
  other.release();
}

template <typename T> inline global_ref<T>::~global_ref() { release(); }

template <typename T> inline void global_ref<T>::release() {
  environment::current().delete_global_ref(_ref);
  _ref = nullptr;
}

template <typename T>
inline global_ref<T> &global_ref<T>::operator=(const global_ref<T> &other) {
  release();
  _ref = new_ref(other._ref);
  return *this;
}

template <typename T>
inline global_ref<T> &global_ref<T>::operator=(global_ref<T> &&other) {
  release();
  _ref = other._ref;
  other._ref = nullptr;
  return *this;
}

template <typename T>
inline global_ref<T> &global_ref<T>::operator=(const local_ref<T> &other) {
  release();
  _ref = new_ref(other.raw());
  return *this;
}

template <typename T>
inline global_ref<T> &global_ref<T>::operator=(local_ref<T> &&other) {
  release();
  _ref = new_ref(other.raw());
  other.release();
  return *this;
}

template <typename T> inline bool global_ref<T>::operator!() { return !_ref; }

template <typename T> inline void global_ref<T>::assign(T ref) {
  release();
  _ref = ref;
}

template <typename T> inline T global_ref<T>::raw() const { return _ref; }

template <typename T> inline T global_ref<T>::leak() {
  auto copy = _ref;
  _ref = nullptr;
  return copy;
}

template <typename T> inline T global_ref<T>::new_ref(T ref) {
  return static_cast<T>(environment::current().new_global_ref(ref));
}

template <typename T>
inline bool operator==(const global_ref<T> &ref, std::nullptr_t) {
  return ref.raw() == nullptr;
}

template <typename T>
inline bool operator==(std::nullptr_t, const global_ref<T> &ref) {
  return ref.raw() == nullptr;
}

template <typename T>
inline bool operator!=(const global_ref<T> &ref, std::nullptr_t) {
  return ref.raw() != nullptr;
}

template <typename T>
inline bool operator!=(std::nullptr_t, const global_ref<T> &ref) {
  return ref.raw() != nullptr;
}

template JNI_EXPORT class global_ref<raw::object_ref>;
template JNI_EXPORT class global_ref<raw::class_ref>;
template JNI_EXPORT class global_ref<raw::string_ref>;
template JNI_EXPORT class global_ref<raw::throwable_ref>;
template JNI_EXPORT class global_ref<raw::typed_array_ref<bool>>;
template JNI_EXPORT class global_ref<raw::typed_array_ref<std::uint8_t>>;
template JNI_EXPORT class global_ref<raw::typed_array_ref<std::uint16_t>>;
template JNI_EXPORT class global_ref<raw::typed_array_ref<std::int16_t>>;
template JNI_EXPORT class global_ref<raw::typed_array_ref<std::int32_t>>;
template JNI_EXPORT class global_ref<raw::typed_array_ref<std::int64_t>>;
template JNI_EXPORT class global_ref<raw::typed_array_ref<float>>;
template JNI_EXPORT class global_ref<raw::typed_array_ref<double>>;
template JNI_EXPORT class global_ref<raw::typed_array_ref<raw::object_ref>>;
template JNI_EXPORT class global_ref<raw::typed_array_ref<raw::class_ref>>;
template JNI_EXPORT class global_ref<raw::typed_array_ref<raw::string_ref>>;
template JNI_EXPORT class global_ref<raw::typed_array_ref<raw::throwable_ref>>;

template JNI_EXPORT global_ref<raw::object_ref>::global_ref(local_ref<raw::object_ref> &&ref);
template JNI_EXPORT global_ref<raw::class_ref>::global_ref(local_ref<raw::class_ref> &&ref);
template JNI_EXPORT global_ref<raw::string_ref>::global_ref(local_ref<raw::string_ref> &&ref);
template JNI_EXPORT global_ref<raw::throwable_ref>::global_ref(local_ref<raw::throwable_ref> &&ref);
template JNI_EXPORT global_ref<raw::typed_array_ref<bool>>::global_ref(local_ref<raw::typed_array_ref<bool>> &&ref);
template JNI_EXPORT global_ref<raw::typed_array_ref<std::uint8_t>>::global_ref(local_ref<raw::typed_array_ref<std::uint8_t>> &&ref);
template JNI_EXPORT global_ref<raw::typed_array_ref<std::uint16_t>>::global_ref(local_ref<raw::typed_array_ref<std::uint16_t>> &&ref);
template JNI_EXPORT global_ref<raw::typed_array_ref<std::int16_t>>::global_ref(local_ref<raw::typed_array_ref<std::int16_t>> &&ref);
template JNI_EXPORT global_ref<raw::typed_array_ref<std::int32_t>>::global_ref(local_ref<raw::typed_array_ref<std::int32_t>> &&ref);
template JNI_EXPORT global_ref<raw::typed_array_ref<std::int64_t>>::global_ref(local_ref<raw::typed_array_ref<std::int64_t>> &&ref);
template JNI_EXPORT global_ref<raw::typed_array_ref<float>>::global_ref(local_ref<raw::typed_array_ref<float>> &&ref);
template JNI_EXPORT global_ref<raw::typed_array_ref<double>>::global_ref(local_ref<raw::typed_array_ref<double>> &&ref);
template JNI_EXPORT global_ref<raw::typed_array_ref<raw::object_ref>>::global_ref(local_ref<raw::typed_array_ref<raw::object_ref>> &&ref);
template JNI_EXPORT global_ref<raw::typed_array_ref<raw::class_ref>>::global_ref(local_ref<raw::typed_array_ref<raw::class_ref>> &&ref);
template JNI_EXPORT global_ref<raw::typed_array_ref<raw::string_ref>>::global_ref(local_ref<raw::typed_array_ref<raw::string_ref>> &&ref);
template JNI_EXPORT global_ref<raw::typed_array_ref<raw::throwable_ref>>::global_ref(local_ref<raw::typed_array_ref<raw::throwable_ref>> &&ref);


} // namespace jni
