
#pragma once

#include <jni/config.hpp>
#include <jni/global_ref.hpp>
#include <jni/local_ref.hpp>
#include <jni/raw/types.hpp>

#include <iosfwd>
#include <string>

namespace jni {

class JNI_EXPORT wstring {
public:
  using value_type = wchar_t;
  using const_pointer = const value_type *;
  using const_iterator = const value_type *;

  explicit wstring(const_pointer characters);
  explicit wstring(local_ref<raw::string_ref> &&ref);
  explicit wstring(global_ref<raw::string_ref> &&ref);
  ~wstring();

  bool loaded() const;

  void load();
  void release();

  bool empty() const;

  const_pointer c_str() const;
  const_pointer data() const;
  std::size_t length() const;

  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

  friend JNI_EXPORT raw::string_ref extract_reference(const wstring &str);
  friend JNI_EXPORT raw::string_ref extract_reference(const wstring &str);

private:
  global_ref<raw::string_ref> _ref;
  mutable std::pair<const_pointer, std::size_t> _buffer;
};

JNI_EXPORT bool operator==(const wstring &left, const wstring &right);

JNI_EXPORT bool operator!=(const wstring &left, const wstring &right);

JNI_EXPORT bool operator==(const wstring &left, const wchar_t *right);
JNI_EXPORT bool operator==(const wchar_t *left, const wstring &right);

JNI_EXPORT bool operator!=(const wstring &left, const wchar_t *right);
JNI_EXPORT bool operator!=(const wchar_t *left, const wstring &right);

JNI_EXPORT bool operator==(const wstring &left, const std::wstring &right);
JNI_EXPORT bool operator==(const std::wstring &left, const wstring &right);

JNI_EXPORT bool operator!=(const wstring &left, const std::wstring &right);
JNI_EXPORT bool operator!=(const std::wstring &left, const wstring &right);

JNI_EXPORT std::wstring to_wstring(const wstring &str);
JNI_EXPORT std::wstring to_wstring(local_ref<raw::string_ref> str);

JNI_EXPORT std::ostream &operator<<(std::ostream &os, const wstring &str);
JNI_EXPORT std::wostream &operator<<(std::wostream &os, const wstring &str);

} // namespace jni
