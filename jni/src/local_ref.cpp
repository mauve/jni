
#include <jni/local_ref.hpp>

#include <jni/global_ref.hpp>
#include <jni/environment.hpp>

namespace jni {

template <typename T> local_ref<T>::local_ref() : _ref(nullptr) {}

template <typename T> local_ref<T>::local_ref(std::nullptr_t) : _ref(nullptr) {}

template <typename T> local_ref<T>::local_ref(T ref) : _ref(ref) {}

template <typename T>
local_ref<T>::local_ref(const local_ref &other)
    : _ref(new_ref(other._ref)) {}

template <typename T>
local_ref<T>::local_ref(local_ref &&other)
    : _ref(other.leak()) {}

template <typename T>
local_ref<T>::local_ref(global_ref<T> &&other)
    : _ref(new_ref(other.raw())) {
  other.release();
}

template <typename T> local_ref<T>::~local_ref() { release(); }

template <typename T> void local_ref<T>::release() {
  environment::current().delete_local_ref(_ref);
  _ref = nullptr;
}

template <typename T>
local_ref<T> &local_ref<T>::operator=(const local_ref &other) {
  release();
  _ref = new_ref(other._ref);
  return *this;
}

template <typename T> local_ref<T> &local_ref<T>::operator=(local_ref &&other) {
  release();
  _ref = other.leak();
  return *this;
}

template <typename T>
local_ref<T> &local_ref<T>::operator=(const global_ref<T> &other) {
  release();
  _ref = new_ref(other.raw());
  return *this;
}

template <typename T>
local_ref<T> &local_ref<T>::operator=(global_ref<T> &&other) {
  release();
  _ref = new_ref(other.leak());
  return *this;
}

template <typename T> void local_ref<T>::assign(T ref) {
  release();
  _ref = ref;
}

template <typename T> T local_ref<T>::raw() const { return _ref; }

template <typename T> T local_ref<T>::leak() {
  auto copy = _ref;
  _ref = nullptr;
  return copy;
}

template <typename T> bool local_ref<T>::operator!() { return !_ref; }

template <typename T> T local_ref<T>::new_ref(T ref) {
  return static_cast<T>(environment::current().new_local_ref(ref));
}

template <typename T> inline local_ref<T> make_local_ref(T ref) {
  return local_ref<T>{ref};
}

template JNI_EXPORT class local_ref<raw::object_ref>;
template JNI_EXPORT class local_ref<raw::class_ref>;
template JNI_EXPORT class local_ref<raw::string_ref>;
template JNI_EXPORT class local_ref<raw::throwable_ref>;
template JNI_EXPORT class local_ref<raw::typed_array_ref<bool>>;
template JNI_EXPORT class local_ref<raw::typed_array_ref<std::uint8_t>>;
template JNI_EXPORT class local_ref<raw::typed_array_ref<std::uint16_t>>;
template JNI_EXPORT class local_ref<raw::typed_array_ref<std::int16_t>>;
template JNI_EXPORT class local_ref<raw::typed_array_ref<std::int32_t>>;
template JNI_EXPORT class local_ref<raw::typed_array_ref<std::int64_t>>;
template JNI_EXPORT class local_ref<raw::typed_array_ref<float>>;
template JNI_EXPORT class local_ref<raw::typed_array_ref<double>>;
template JNI_EXPORT class local_ref<raw::typed_array_ref<raw::object_ref>>;
template JNI_EXPORT class local_ref<raw::typed_array_ref<raw::class_ref>>;
template JNI_EXPORT class local_ref<raw::typed_array_ref<raw::string_ref>>;
template JNI_EXPORT class local_ref<raw::typed_array_ref<raw::throwable_ref>>;

template JNI_EXPORT local_ref<raw::object_ref> make_local_ref(raw::object_ref ref);
template JNI_EXPORT local_ref<raw::class_ref> make_local_ref(raw::class_ref ref);
template JNI_EXPORT local_ref<raw::string_ref> make_local_ref(raw::string_ref ref);
template JNI_EXPORT local_ref<raw::throwable_ref> make_local_ref(raw::throwable_ref ref);
template JNI_EXPORT local_ref<raw::typed_array_ref<bool>> make_local_ref(raw::typed_array_ref<bool> ref);
template JNI_EXPORT local_ref<raw::typed_array_ref<std::uint8_t>> make_local_ref(raw::typed_array_ref<std::uint8_t> ref);
template JNI_EXPORT local_ref<raw::typed_array_ref<std::uint16_t>> make_local_ref(raw::typed_array_ref<std::uint16_t> ref);
template JNI_EXPORT local_ref<raw::typed_array_ref<std::int16_t>> make_local_ref(raw::typed_array_ref<std::int16_t> ref);
template JNI_EXPORT local_ref<raw::typed_array_ref<std::int32_t>> make_local_ref(raw::typed_array_ref<std::int32_t> ref);
template JNI_EXPORT local_ref<raw::typed_array_ref<std::int64_t>> make_local_ref(raw::typed_array_ref<std::int64_t> ref);
template JNI_EXPORT local_ref<raw::typed_array_ref<float>> make_local_ref(raw::typed_array_ref<float> ref);
template JNI_EXPORT local_ref<raw::typed_array_ref<double>> make_local_ref(raw::typed_array_ref<double> ref);
template JNI_EXPORT local_ref<raw::typed_array_ref<raw::object_ref>> make_local_ref(raw::typed_array_ref<raw::object_ref> ref);
template JNI_EXPORT local_ref<raw::typed_array_ref<raw::class_ref>> make_local_ref(raw::typed_array_ref<raw::class_ref> ref);
template JNI_EXPORT local_ref<raw::typed_array_ref<raw::string_ref>> make_local_ref(raw::typed_array_ref<raw::string_ref> ref);
template JNI_EXPORT local_ref<raw::typed_array_ref<raw::throwable_ref>> make_local_ref(raw::typed_array_ref<raw::throwable_ref> ref);

} // namespace jni
