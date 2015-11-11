
#include <jni/array.hpp>

#include <jni/config.hpp>
#include <jni/environment.hpp>

namespace jni {

template <typename T>
inline array<T>::array(std::size_t size)
    : _ref(environment::current().new_array(size,
                                            reinterpret_cast<T *>(nullptr))),
      _elements(nullptr, environment::current().get_array_size(_ref.raw())) {}

template <typename T>
inline array<T>::array(local_ref<raw::typed_array_ref<T>> &&ref)
    : _ref(std::move(ref)),
      _elements(nullptr, environment::current().get_array_size(_ref.raw())) {}

template <typename T>
inline array<T>::array(array &&other)
    : _ref(std::move(other._ref)), _elements(std::move(other._elements)) {
  other._elements.first = nullptr;
  other._elements.second = 0;
}

template <typename T> inline array<T>::~array() { abort(); }

template <typename T> inline array<T> &array<T>::operator=(array &&other) {
  abort();
  _ref = std::move(other._ref);
  _elements = std::move(other._elements);
  other._elements.first = nullptr;
  other._elements.second = 0;
  return *this;
}

template <typename T> inline void array<T>::load() {
  if (loaded())
    return;

  _elements = environment::current().get_array_elements(_ref.raw());
}

template <typename T> inline void array<T>::abort() {
  if (!loaded())
    return;

  environment::current().release_array_elements(
      _ref.raw(), _elements.first, environment::array_release_op::free_buffer);

  _elements.first = nullptr;
}

template <typename T> inline void array<T>::commit() {
  if (!loaded())
    return;

  environment::current().release_array_elements(
      _ref.raw(), _elements.first,
      environment::array_release_op::copy_and_free_buffer);

  _elements.first = nullptr;
}

template <typename T> inline std::size_t array<T>::size() const {
  return _elements.second;
}

template <typename T> T &array<T>::operator[](std::size_t index) {
  load();
#ifdef _DEBUG
  if (index >= _elements.second) {
    throw std::out_of_range("index out range in jni::array<T>::operator[]");
  }
#endif
  return _elements.first[index];
}

template <typename T> T array<T>::operator[](std::size_t index) const {
  const_cast<array<T> *>(this)->load();
#ifdef _DEBUG
  if (index >= _elements.second) {
    throw std::out_of_range("index out range in jni::array<T>::operator[]");
  }
#endif
  return _elements.first[index];
}

template <typename T> typename array<T>::iterator array<T>::begin() {
  load();
  return data();
}

template <typename T> typename array<T>::iterator array<T>::end() {
  load();
  return data() + _elements.second;
}

template <typename T>
typename array<T>::const_iterator array<T>::begin() const {
  const_cast<array<T> *>(this)->load();
  return data();
}

template <typename T> typename array<T>::const_iterator array<T>::end() const {
  const_cast<array<T> *>(this)->load();
  return data() + _elements.second;
}

template <typename T>
typename array<T>::const_iterator array<T>::cbegin() const {
  return begin();
}

template <typename T> typename array<T>::const_iterator array<T>::cend() const {
  return end();
}

template <typename T> T *array<T>::data() {
  load();
  return _elements.first;
}

template <typename T> const T *array<T>::data() const {
  const_cast<array<T> *>(this)->load();
  return _elements.first;
}

template <typename T> bool array<T>::loaded() const {
  return _elements.first != nullptr;
}

template <typename T>
inline raw::typed_array_ref<T> extract_reference(const array<T> &arr) {
  return arr._ref.raw();
}

template JNI_EXPORT class array<bool>;
template JNI_EXPORT class array<std::uint8_t>;
template JNI_EXPORT class array<std::uint16_t>;
template JNI_EXPORT class array<std::int16_t>;
template JNI_EXPORT class array<std::int32_t>;
template JNI_EXPORT class array<std::int64_t>;
template JNI_EXPORT class array<float>;
template JNI_EXPORT class array<double>;

template JNI_EXPORT raw::typed_array_ref<bool> extract_reference(const array<bool> &);
template JNI_EXPORT raw::typed_array_ref<std::uint8_t> extract_reference(const array<std::uint8_t> &);
template JNI_EXPORT raw::typed_array_ref<std::uint16_t> extract_reference(const array<std::uint16_t> &);
template JNI_EXPORT raw::typed_array_ref<std::int16_t> extract_reference(const array<std::int16_t> &);
template JNI_EXPORT raw::typed_array_ref<std::int32_t> extract_reference(const array<std::int32_t> &);
template JNI_EXPORT raw::typed_array_ref<std::int64_t> extract_reference(const array<std::int64_t> &);
template JNI_EXPORT raw::typed_array_ref<float> extract_reference(const array<float> &);
template JNI_EXPORT raw::typed_array_ref<double> extract_reference(const array<double> &);

} // namespace jni