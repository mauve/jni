
#pragma once

#include <jni/ref.hpp>
#include <jni/raw/types.hpp>

#include <cstddef>
#include <utility>

namespace jni {

template <typename T> class array {
public:
  using value_type = T;
  using iterator = T*;
  using const_iterator = const T*;

  explicit array(std::size_t size);
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

private:
  local_ref<raw::typed_array_ref<T>> _ref;
  std::pair<T*, std::size_t> _elements;
};

} // namespace jni

#define ARRAY_INCLUDING
# include <jni/impl/array.ipp>
#undef ARRAY_INCLUDING
