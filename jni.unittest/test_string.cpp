
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <jni/environment.hpp>
#include <jni/string.hpp>
#include <jni/wstring.hpp>

#include <string>
#include <sstream>

template <typename T> struct test_data;

template <> struct test_data<jni::string> {
  using std_string_type = std::string;
  using std_stream_type = std::stringstream;

  static const char *constant1() { return "constant1"; }
  static const char *constant2() { return "constant2"; }
  static int length() { return 9; }
  static const char *empty() { return ""; }
};

template <> struct test_data<jni::wstring> {
  using std_string_type = std::wstring;
  using std_stream_type = std::wstringstream;

  static const wchar_t *constant1() { return L"constant1"; }
  static const wchar_t *constant2() { return L"constant2"; }
  static int length() { return 9; }
  static const wchar_t *empty() { return L""; }
};

using jni_strings = boost::mpl::list<jni::string, jni::wstring>;

BOOST_AUTO_TEST_SUITE(string_tests)

BOOST_AUTO_TEST_CASE_TEMPLATE(create_from_string, T, jni_strings) {
  T string{test_data<T>::constant1()};

  BOOST_TEST(!string.loaded());

  string.load();
  BOOST_TEST(string.loaded());

  BOOST_TEST(string == test_data<T>::constant1());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(release_and_load, T, jni_strings) {
  T string{test_data<T>::constant1()};

  BOOST_TEST(!string.loaded());

  string.load();

  BOOST_TEST(string.loaded());
  BOOST_TEST(string == test_data<T>::constant1());

  string.release();

  BOOST_TEST(!string.loaded());

  string.load();

  BOOST_TEST(string.loaded());
  BOOST_TEST(string == test_data<T>::constant1());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(compare, T, jni_strings) {
  T constant1{test_data<T>::constant1()};
  T constant1_copy{test_data<T>::constant1()};
  T constant2{test_data<T>::constant2()};
  test_data<T>::std_string_type std_constant1 = test_data<T>::constant1();

  BOOST_TEST(constant1 == constant1_copy);
  BOOST_TEST(constant1 == test_data<T>::constant1());
  BOOST_TEST(test_data<T>::constant1() == constant1);
  bool strings_are_equal = constant1 == std_constant1;
  BOOST_TEST(strings_are_equal);
  strings_are_equal = std_constant1 == constant1;
  BOOST_TEST(strings_are_equal);

  BOOST_TEST(constant1 != constant2);
  BOOST_TEST(constant1 != test_data<T>::constant2());
  BOOST_TEST(test_data<T>::constant2() != constant1);
  bool strings_are_not_equal = constant2 != std_constant1;
  BOOST_TEST(strings_are_not_equal);
  strings_are_not_equal = std_constant1 != constant2;
  BOOST_TEST(strings_are_not_equal);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(empty, T, jni_strings) {
  T string{test_data<T>::empty()};
  T string1{test_data<T>::constant1()};

  BOOST_TEST(!string.loaded());

  BOOST_TEST(string.empty());
  BOOST_TEST(!string1.empty());

  BOOST_TEST(string.loaded());

  BOOST_TEST(string.length() == 0);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(length, T, jni_strings) {
  T string{test_data<T>::constant1()};

  BOOST_TEST(!string.loaded());

  BOOST_TEST(string.length() == test_data<T>::length());

  BOOST_TEST(string.loaded());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(output, T, jni_strings) {
  T string{test_data<T>::constant1()};

  test_data<T>::std_stream_type stream;
  stream << string;

  auto result = stream.str();

  // BOOST_TEST doesnt work with wstring
  bool strings_are_equal = result == string.c_str();
  BOOST_TEST(strings_are_equal);
}

BOOST_AUTO_TEST_CASE(to_string) {
  jni::string string{"constant1"};

  using jni::to_string;

  auto result = to_string(string);
  BOOST_TEST(result == "constant1");
}

BOOST_AUTO_TEST_CASE(to_wstring) {
  jni::wstring string{L"constant1"};

  using jni::to_wstring;

  auto result = to_wstring(string);
  // BOOST_TEST doesnt work with wstring
  bool strings_are_equal = result == L"constant1";
  BOOST_TEST(strings_are_equal);
}

BOOST_AUTO_TEST_CASE(to_string_raw) {
  auto raw = jni::environment::current().new_string("constant1");

  using jni::to_string;

  auto result = to_string(raw);
  BOOST_TEST(result == "constant1");
}

BOOST_AUTO_TEST_CASE(to_wstring_raw) {
  auto raw = jni::environment::current().new_string(L"constant1");

  using jni::to_wstring;

  auto result = to_wstring(raw);

  // BOOST_TEST doesnt work with wstring
  bool strings_are_equal = result == L"constant1";
  BOOST_TEST(strings_are_equal);
}

BOOST_AUTO_TEST_SUITE_END()
