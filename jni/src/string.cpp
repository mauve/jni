
#include <jni/string.hpp>
#include <jni/environment.hpp>
#include <jni/detail/throw_if_exception.hpp>

#include <cstring>
#include <iostream>

namespace jni {

string::string(const_pointer characters)
    : _ref{environment::current().new_string(characters)}, _buffer{nullptr, 0} {
  throw_if_exception();
}

string::string(local_ref<raw::string_ref> &&ref)
    : _ref{std::move(ref)}, _buffer{nullptr, 0} {}

string::string(global_ref<raw::string_ref> &&ref)
    : _ref{std::move(ref)}, _buffer{nullptr, 0} {}

string::~string() { release(); }

bool string::loaded() const { return _buffer.first != 0; }

void string::load() {
  if (loaded()) {
    return;
  }

  auto buffer = environment::current().load_utf8_characters(_ref.raw());
  throw_if_exception();
  _buffer = buffer;
}

void string::release() {
  if (!loaded()) {
    return;
  }

  environment::current().release_string_elements(_ref.raw(), _buffer);
  _buffer = std::make_pair(nullptr, 0);
  throw_if_exception();
}

bool string::empty() const { return length() == 0; }

string::const_iterator string::c_str() const {
  return data();
}

string::const_pointer string::data() const {
  const_cast<string *>(this)->load();
  return _buffer.first;
}

std::size_t string::length() const {
  const_cast<string *>(this)->load();
  return _buffer.second;
}

string::const_iterator string::begin() const { return data(); }

string::const_iterator string::end() const { return data() + length(); }

string::const_iterator string::cbegin() const { return data(); }

string::const_iterator string::cend() const { return data() + length(); }

bool operator==(const string &left, const string &right) {
  return std::strcmp(left.c_str(), right.c_str()) == 0;
}

bool operator!=(const string &left, const string &right) {
  return std::strcmp(left.c_str(), right.c_str()) != 0;
}

bool operator==(const string &left, const char *right) {
  return std::strcmp(left.c_str(), right) == 0;
}

bool operator==(const char *left, const string &right) {
  return std::strcmp(left, right.c_str()) == 0;
}

bool operator!=(const string &left, const char *right) {
  return std::strcmp(left.c_str(), right) != 0;
}

bool operator!=(const char *left, const string &right) {
  return std::strcmp(left, right.c_str()) != 0;
}

bool operator==(const string &left, const std::string &right) {
  return left == right.c_str();
}

bool operator==(const std::string &left, const string &right) {
  return left.c_str() == right;
}

bool operator!=(const string &left, const std::string &right) {
  return left != right.c_str();
}

bool operator!=(const std::string &left, const string &right) {
  return left.c_str() != right;
}

std::string to_string(const string &str) { return str.c_str(); }

std::string to_string(raw::string_ref str) {
  auto buffer = environment::current().load_utf8_characters(str);
  throw_if_exception();
  std::string result{buffer.first};
  environment::current().release_string_elements(str, buffer);
  throw_if_exception();
  return result;
}

std::string to_string(local_ref<raw::string_ref> ref) {
  return to_string(ref.raw());
}

std::ostream &operator<<(std::ostream &os, const string &str) {
  return os << str.c_str();
}

raw::string_ref extract_reference(const string &str) { return str._ref.raw(); }

} // namespace jni
