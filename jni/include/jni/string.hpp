
#pragma once

#include <jni/config.hpp>
#include <jni/environment.hpp>
#include <jni/ref.hpp>
#include <jni/raw/types.hpp>

#include <iosfwd>
#include <string>

namespace jni {

namespace impl {

struct JNI_EXPORT modified_utf8 {
  using value_type = const char;

  static local_ref<raw::string_ref> create(value_type *begin);
  static std::pair<value_type *, std::size_t> load(raw::string_ref ref);
  static void release(raw::string_ref ref,
                      const std::pair<value_type *, std::size_t> &buffer);
};

struct JNI_EXPORT modified_utf16 {
  using value_type = const wchar_t;

  static local_ref<raw::string_ref> create(value_type *begin);
  static std::pair<value_type *, std::size_t> load(raw::string_ref ref);
  static void release(raw::string_ref ref,
                      const std::pair<value_type *, std::size_t> &buffer);
};

} // namespace impl

template <typename Impl> class basic_string {
public:
  using implementation_type = Impl;

  using value_type = typename Impl::value_type;
  using pointer = value_type *;
  using const_iterator = value_type *;

  explicit basic_string(pointer characters);
  explicit basic_string(local_ref<raw::string_ref> &&ref);
  explicit basic_string(global_ref<raw::string_ref> &&ref);
  ~basic_string();

  bool loaded() const;
  void load();
  void release();

  bool empty() const;

  pointer c_str() const;
  pointer data() const;
  std::size_t length() const;

  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

  raw::string_ref ref();

private:
  global_ref<raw::string_ref> _ref;
  mutable std::pair<pointer, std::size_t> _buffer;
};

JNI_EXPORT bool operator==(const basic_string<impl::modified_utf8> &left,
                           const basic_string<impl::modified_utf8> &right);

JNI_EXPORT bool operator!=(const basic_string<impl::modified_utf8> &left,
                           const basic_string<impl::modified_utf8> &right);

JNI_EXPORT bool operator==(const basic_string<impl::modified_utf16> &left,
                           const basic_string<impl::modified_utf16> &right);

JNI_EXPORT bool operator!=(const basic_string<impl::modified_utf16> &left,
                           const basic_string<impl::modified_utf16> &right);

JNI_EXPORT bool operator==(const basic_string<impl::modified_utf8> &left,
                           const char *right);
JNI_EXPORT bool operator==(const char *left,
                           const basic_string<impl::modified_utf8> &right);

JNI_EXPORT bool operator!=(const basic_string<impl::modified_utf8> &left,
                           const char *right);
JNI_EXPORT bool operator!=(const char *left,
                           const basic_string<impl::modified_utf8> &right);

JNI_EXPORT bool operator==(const basic_string<impl::modified_utf16> &left,
                           const wchar_t *right);
JNI_EXPORT bool operator==(const wchar_t *left,
                           const basic_string<impl::modified_utf16> &right);

JNI_EXPORT bool operator!=(const basic_string<impl::modified_utf16> &left,
                           const wchar_t *right);
JNI_EXPORT bool operator!=(const wchar_t *left,
                           const basic_string<impl::modified_utf16> &right);

JNI_EXPORT bool operator==(const basic_string<impl::modified_utf8> &left,
                           const std::string &right);
JNI_EXPORT bool operator==(const std::string &left,
                           const basic_string<impl::modified_utf8> &right);

JNI_EXPORT bool operator!=(const basic_string<impl::modified_utf8> &left,
                           const std::string &right);
JNI_EXPORT bool operator!=(const std::string &left,
                           const basic_string<impl::modified_utf8> &right);

JNI_EXPORT bool operator==(const basic_string<impl::modified_utf16> &left,
                           const std::wstring &right);
JNI_EXPORT bool operator==(const std::wstring &left,
                           const basic_string<impl::modified_utf16> &right);

JNI_EXPORT bool operator!=(const basic_string<impl::modified_utf16> &left,
                           const std::wstring &right);
JNI_EXPORT bool operator!=(const std::wstring &left,
                           const basic_string<impl::modified_utf16> &right);

JNI_EXPORT std::string to_string(const basic_string<impl::modified_utf8>& str);
JNI_EXPORT std::wstring to_wstring(const basic_string<impl::modified_utf16>& str);

JNI_EXPORT std::string to_string(local_ref<raw::string_ref> str);
JNI_EXPORT std::wstring to_wstring(local_ref<raw::string_ref> str);

JNI_EXPORT std::ostream &
operator<<(std::ostream &os, const basic_string<impl::modified_utf8> &str);

JNI_EXPORT std::ostream &
operator<<(std::ostream &os, const basic_string<impl::modified_utf16> &str);

JNI_EXPORT std::wostream &
operator<<(std::wostream &os, const basic_string<impl::modified_utf16> &str);

JNI_EXPORT std::wostream &
operator<<(std::wostream &os, const basic_string<impl::modified_utf8> &str);

using modified_utf8_string = basic_string<impl::modified_utf8>;
using modified_utf16_string = basic_string<impl::modified_utf16>;

} // namespace jni

#define STRING_INCLUDING
#include <jni/impl/string.ipp>
#undef STRING_INCLUDING
