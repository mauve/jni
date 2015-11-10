
#include <jni/string.hpp>

#include <jni/environment.hpp>

#include <cstring>
#include <iostream>

namespace jni {
namespace impl {

local_ref<raw::string_ref> modified_utf8::create(value_type *characters) {
  return environment::current().new_string(characters);
}

std::pair<modified_utf8::value_type *, std::size_t>
modified_utf8::load(raw::string_ref ref) {
  if (!ref) {
    return {nullptr, 0};
  }

  return environment::current().load_utf8_characters(ref);
}

void modified_utf8::release(
    raw::string_ref ref, const std::pair<value_type *, std::size_t> &buffer) {
  if (!ref || !buffer.first)
    return;

  return environment::current().release_string_elements(ref, buffer);
}

local_ref<raw::string_ref> modified_utf16::create(value_type *characters) {
  return environment::current().new_string(characters);
}

std::pair<modified_utf16::value_type *, std::size_t>
modified_utf16::load(raw::string_ref ref) {
  if (!ref) {
    return {nullptr, 0};
  }

  return environment::current().load_utf16_characters(ref);
}

void modified_utf16::release(
    raw::string_ref ref, const std::pair<value_type *, std::size_t> &buffer) {
  if (!ref || !buffer.first)
    return;

  return environment::current().release_string_elements(ref, buffer);
}

} // namespace impl

bool operator==(const basic_string<impl::modified_utf8> &left,
                const basic_string<impl::modified_utf8> &right) {
  return std::strcmp(left.c_str(), right.c_str()) == 0;
}

bool operator!=(const basic_string<impl::modified_utf8> &left,
                const basic_string<impl::modified_utf8> &right) {
  return std::strcmp(left.c_str(), right.c_str()) != 0;
}

bool operator==(const basic_string<impl::modified_utf16> &left,
                const basic_string<impl::modified_utf16> &right) {
  return std::wcscmp(left.c_str(), right.c_str()) == 0;
}

bool operator!=(const basic_string<impl::modified_utf16> &left,
                const basic_string<impl::modified_utf16> &right) {
  return std::wcscmp(left.c_str(), right.c_str()) != 0;
}

bool operator==(const basic_string<impl::modified_utf8> &left,
                const char *right) {
  return std::strcmp(left.c_str(), right) == 0;
}

bool operator==(const char *left,
                const basic_string<impl::modified_utf8> &right) {
  return std::strcmp(left, right.c_str()) == 0;
}

bool operator!=(const basic_string<impl::modified_utf8> &left,
                const char *right) {
  return std::strcmp(left.c_str(), right) != 0;
}

bool operator!=(const char *left,
                const basic_string<impl::modified_utf8> &right) {
  return std::strcmp(left, right.c_str()) != 0;
}

bool operator==(const basic_string<impl::modified_utf16> &left,
                const wchar_t *right) {
  return std::wcscmp(left.c_str(), right) == 0;
}
bool operator==(const wchar_t *left,
                const basic_string<impl::modified_utf16> &right) {
  return std::wcscmp(left, right.c_str()) == 0;
}

bool operator!=(const basic_string<impl::modified_utf16> &left,
                const wchar_t *right) {
  return std::wcscmp(left.c_str(), right) != 0;
}

bool operator!=(const wchar_t *left,
                const basic_string<impl::modified_utf16> &right) {
  return std::wcscmp(left, right.c_str()) != 0;
}

bool operator==(const basic_string<impl::modified_utf8> &left,
                const std::string &right) {
  return left == right.c_str();
}

bool operator==(const std::string &left,
                const basic_string<impl::modified_utf8> &right) {
  return left.c_str() == right;
}

bool operator!=(const basic_string<impl::modified_utf8> &left,
                const std::string &right) {
  return left != right.c_str();
}

bool operator!=(const std::string &left,
                const basic_string<impl::modified_utf8> &right) {
  return left.c_str() != right;
}

bool operator==(const basic_string<impl::modified_utf16> &left,
                const std::wstring &right) {
  return left == right.c_str();
}

bool operator==(const std::wstring &left,
                const basic_string<impl::modified_utf16> &right) {
  return left.c_str() == right;
}

bool operator!=(const basic_string<impl::modified_utf16> &left,
                const std::wstring &right) {
  return left != right.c_str();
}

bool operator!=(const std::wstring &left,
                const basic_string<impl::modified_utf16> &right) {
  return left.c_str() != right;
}

std::string to_string(const basic_string<impl::modified_utf8> &str) {
  return str.c_str();
}

std::wstring to_wstring(const basic_string<impl::modified_utf16> &str) {
  return str.c_str();
}

std::string to_string(raw::string_ref str) {
  auto buffer = impl::modified_utf8::load(str);
  if (!buffer.first) {
    // TODO: throw exception
    return "";
  }

  std::string result{buffer.first};

  impl::modified_utf8::release(str, buffer);

  return result;
}

std::string to_string(local_ref<raw::string_ref> ref) {
  return to_string(ref.raw());
}

std::wstring to_wstring(raw::string_ref str) {
  auto buffer = impl::modified_utf16::load(str);
  if (!buffer.first) {
    // TODO: throw exception
    return L"";
  }

  std::wstring result{buffer.first};

  impl::modified_utf16::release(str, buffer);

  return result;
}

std::wstring to_wstring(local_ref<raw::string_ref> ref) {
  return to_wstring(ref.raw());
}

std::ostream &operator<<(std::ostream &os,
                         const basic_string<impl::modified_utf8> &str) {
  return os << str.c_str();
}

std::ostream &operator<<(std::ostream &os,
                         const basic_string<impl::modified_utf16> &str) {
  return os << str.c_str();
}

std::wostream &operator<<(std::wostream &os,
                          const basic_string<impl::modified_utf16> &str) {
  return os << str.c_str();
}

std::wostream &operator<<(std::wostream &os,
                          const basic_string<impl::modified_utf8> &str) {
  return os << str.c_str();
}

raw::string_ref extract_reference(const basic_string<impl::modified_utf8> &str) {
  return str._ref.raw();
}

raw::string_ref extract_reference(const basic_string<impl::modified_utf16> &str) {
  return str._ref.raw();
}

} // namespace jni
