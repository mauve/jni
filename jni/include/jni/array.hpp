
#pragma once

#include <jni/local_ref.hpp>
#include <jni/raw/types.hpp>

#include <cstddef>
#include <utility>

namespace jni {

template <typename T> class JNI_EXPORT array {
public:
  using value_type = T;
  using iterator = T*;
  using const_iterator = const T*;

  explicit array(std::size_t size);
  explicit array(const_iterator begin, const_iterator end);
  explicit array(local_ref<raw::typed_array_ref<T>> &&ref);
  array(array &&other);
  ~array();

  array(const array &other) = delete;

  array &operator=(const array &other) = delete;
  array &operator=(array &&other);

  void load();
  void abort();
  void commit();

  std::size_t size() const;

  T &operator[](std::size_t index);
  T operator[](std::size_t index) const;

  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

  const_iterator cbegin() const;
  const_iterator cend() const;

  T *data();
  const T *data() const;

  bool loaded() const;

  template <typename T>
  friend raw::typed_array_ref<T> extract_reference(const array<T>& arr);

private:
  local_ref<raw::typed_array_ref<T>> _ref;
  std::pair<T*, std::size_t> _elements;
};

extern template class array<bool>;
extern template class array<std::uint8_t>;
extern template class array<std::uint16_t>;
extern template class array<std::int16_t>;
extern template class array<std::int32_t>;
extern template class array<std::int64_t>;
extern template class array<float>;
extern template class array<double>;

} // namespace jni
