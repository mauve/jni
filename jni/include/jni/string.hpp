
#pragma once

#include <jni/config.hpp>
#include <jni/global_ref.hpp>
#include <jni/local_ref.hpp>
#include <jni/raw/types.hpp>

#include <iosfwd>
#include <string>

namespace jni {

class JNI_EXPORT string {
public:
  using value_type = char;
  using const_pointer = const value_type *;
  using const_iterator = const value_type *;

  explicit string(const_pointer characters);
  explicit string(local_ref<raw::string_ref> &&ref);
  explicit string(global_ref<raw::string_ref> &&ref);
  ~string();

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

  friend JNI_EXPORT raw::string_ref extract_reference(const string &str);
  friend JNI_EXPORT raw::string_ref extract_reference(const string &str);

private:
  global_ref<raw::string_ref> _ref;
  mutable std::pair<const_pointer, std::size_t> _buffer;
};

JNI_EXPORT bool operator==(const string &left, const string &right);

JNI_EXPORT bool operator!=(const string &left, const string &right);

JNI_EXPORT bool operator==(const string &left, const char *right);
JNI_EXPORT bool operator==(const char *left, const string &right);

JNI_EXPORT bool operator!=(const string &left, const char *right);
JNI_EXPORT bool operator!=(const char *left, const string &right);

JNI_EXPORT bool operator==(const string &left, const std::string &right);
JNI_EXPORT bool operator==(const std::string &left, const string &right);

JNI_EXPORT bool operator!=(const string &left, const std::string &right);
JNI_EXPORT bool operator!=(const std::string &left, const string &right);

JNI_EXPORT std::string to_string(const string &str);
JNI_EXPORT std::string to_string(local_ref<raw::string_ref> str);

JNI_EXPORT std::ostream &operator<<(std::ostream &os, const string &str);
JNI_EXPORT std::wostream &operator<<(std::wostream &os, const string &str);

} // namespace jni
