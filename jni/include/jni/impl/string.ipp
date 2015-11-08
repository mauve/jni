
#pragma once

#ifndef STRING_INCLUDING
#error "string.ipp cannot be included directly"
#endif

namespace jni {

template <typename Impl>
inline basic_string<Impl>::basic_string(pointer characters)
    : _ref{implementation_type::create(characters)}, _buffer{nullptr, 0} {}

template <typename Impl>
inline basic_string<Impl>::basic_string(local_ref<raw::string_ref> &&ref)
    : _ref(std::move(ref)), _buffer{nullptr, 0} {}

template <typename Impl>
inline basic_string<Impl>::basic_string(global_ref<raw::string_ref> &&ref)
    : _ref(std::move(ref)), _buffer{nullptr, 0} {}

template <typename Impl> inline basic_string<Impl>::~basic_string() {
  release();
}

template <typename Impl> inline bool basic_string<Impl>::loaded() const {
  return _buffer.first != nullptr;
}

template <typename Impl> inline void basic_string<Impl>::load() {
  if (_buffer.first)
    return;

  _buffer = implementation_type::load(_ref.raw());
}

template <typename Impl> inline void basic_string<Impl>::release() {
  if (!_buffer.first)
    return;

  implementation_type::release(_ref.raw(), _buffer);
  _buffer.first = nullptr;
  _buffer.second = 0;
}

template <typename Impl> inline bool basic_string<Impl>::empty() const {
  const_cast<basic_string<Impl>*>(this)->load();
  return _buffer.second == 0;
}

template <typename Impl>
inline typename basic_string<Impl>::pointer basic_string<Impl>::c_str() const {
  const_cast<basic_string<Impl>*>(this)->load();
  return _buffer.first;
}

template <typename Impl>
inline typename basic_string<Impl>::pointer basic_string<Impl>::data() const {
  const_cast<basic_string<Impl>*>(this)->load();
  return _buffer.first;
}

template <typename Impl> inline std::size_t basic_string<Impl>::length() const {
  const_cast<basic_string<Impl>*>(this)->load();
  return _buffer.second;
}

template <typename Impl>
inline typename basic_string<Impl>::const_iterator basic_string<Impl>::begin() const {
  return data();
}

template <typename Impl>
inline typename basic_string<Impl>::const_iterator basic_string<Impl>::end() const {
  return data() + _buffer.second;
}

template <typename Impl>
inline typename basic_string<Impl>::const_iterator basic_string<Impl>::cbegin() const {
  return begin();
}

template <typename Impl>
inline typename basic_string<Impl>::const_iterator basic_string<Impl>::cend() const {
  return end();
}

template <typename Impl>
inline raw::string_ref basic_string<Impl>::ref() {
  return _ref.raw();
}

} // namespace jni
