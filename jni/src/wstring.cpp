
#include <jni/wstring.hpp>
#include <jni/environment.hpp>
#include <jni/detail/throw_if_exception.hpp>

#include <cstring>
#include <iostream>

namespace jni {

wstring::wstring(const_pointer characters)
    : _ref{environment::current().new_string(characters)}, _buffer{nullptr, 0} {
  throw_if_exception();
}

wstring::wstring(local_ref<raw::string_ref> &&ref)
    : _ref{std::move(ref)}, _buffer{nullptr, 0} {}

wstring::wstring(global_ref<raw::string_ref> &&ref)
    : _ref{std::move(ref)}, _buffer{nullptr, 0} {}

wstring::~wstring() { release(); }

bool wstring::loaded() const { return _buffer.first != 0; }

void wstring::load() {
  if (loaded()) {
    return;
  }

  auto buffer = environment::current().load_utf16_characters(_ref.raw());
  throw_if_exception();
  _buffer = buffer;
}

void wstring::release() {
  if (!loaded()) {
    return;
  }

  environment::current().release_string_elements(_ref.raw(), _buffer);
  _buffer = std::make_pair(nullptr, 0);
  throw_if_exception();
}

bool wstring::empty() const { return length() == 0; }

wstring::const_iterator wstring::c_str() const {
  return data();
}

wstring::const_pointer wstring::data() const {
  const_cast<wstring *>(this)->load();
  return _buffer.first;
}

std::size_t wstring::length() const {
  const_cast<wstring *>(this)->load();
  return _buffer.second;
}

wstring::const_iterator wstring::begin() const { return data(); }

wstring::const_iterator wstring::end() const { return data() + length(); }

wstring::const_iterator wstring::cbegin() const { return data(); }

wstring::const_iterator wstring::cend() const { return data() + length(); }

bool operator==(const wstring &left, const wstring &right) {
  return std::wcscmp(left.c_str(), right.c_str()) == 0;
}

bool operator!=(const wstring &left, const wstring &right) {
  return std::wcscmp(left.c_str(), right.c_str()) != 0;
}

bool operator==(const wstring &left, const wchar_t *right) {
  return std::wcscmp(left.c_str(), right) == 0;
}
bool operator==(const wchar_t *left, const wstring &right) {
  return std::wcscmp(left, right.c_str()) == 0;
}

bool operator!=(const wstring &left, const wchar_t *right) {
  return std::wcscmp(left.c_str(), right) != 0;
}

bool operator!=(const wchar_t *left, const wstring &right) {
  return std::wcscmp(left, right.c_str()) != 0;
}

bool operator==(const wstring &left, const std::wstring &right) {
  return left == right.c_str();
}

bool operator==(const std::wstring &left, const wstring &right) {
  return left.c_str() == right;
}

bool operator!=(const wstring &left, const std::wstring &right) {
  return left != right.c_str();
}

bool operator!=(const std::wstring &left, const wstring &right) {
  return left.c_str() != right;
}

std::wstring to_wstring(const wstring &str) { return str.c_str(); }

std::wstring to_wstring(raw::string_ref str) {
  auto buffer = environment::current().load_utf16_characters(str);
  throw_if_exception();
  std::wstring result{buffer.first};
  environment::current().release_string_elements(str, buffer);
  return result;
}

std::wstring to_wstring(local_ref<raw::string_ref> ref) {
  return to_wstring(ref.raw());
}

std::ostream &operator<<(std::ostream &os, const wstring &str) {
  return os << str.c_str();
}

std::wostream &operator<<(std::wostream &os, const wstring &str) {
  return os << str.c_str();
}

raw::string_ref extract_reference(const wstring &str) { return str._ref.raw(); }

} // namespace jni
