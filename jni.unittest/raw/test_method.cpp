
#include <boost/test/unit_test.hpp>

#include <jni/java/lang/class.hpp>
#include <jni/java/lang/object.hpp>
#include <jni/raw/method.hpp>

#include <initializer_list>
#include <numeric>

template <typename T>
void test_call_method(const char *name, std::initializer_list<T> test_values) {
  auto cls = jni::java::lang::Class::forName("MethodTest");
  auto mid = cls.getMethod<T(T)>(name);
  BOOST_REQUIRE_MESSAGE(mid != nullptr, "no method named " << name
                                                           << " on MethodTest");

  auto instance = cls.newInstance();

  jni::raw::method<T(T)> method{mid};

  for (const auto &value : test_values) {
    auto result = method(jni::environment::current(), instance.ref(), value);

    BOOST_TEST(result == value);
  }
}

BOOST_AUTO_TEST_SUITE(raw_tests)
BOOST_AUTO_TEST_SUITE(method_tests)

BOOST_AUTO_TEST_CASE(call_boolean_method) {
  test_call_method<bool>("booleanMethod", {true, false});
}

BOOST_AUTO_TEST_CASE(call_byte_method) {
  test_call_method<std::uint8_t>("byteMethod", {0, 1, 2, 3, 255});
}

BOOST_AUTO_TEST_CASE(call_char_method) {
  test_call_method<std::uint16_t>(
      "charMethod", {0, 1, 1024, std::numeric_limits<std::uint16_t>::max()});
}

BOOST_AUTO_TEST_CASE(call_short_method) {
  test_call_method<std::int16_t>(
      "shortMethod", {std::numeric_limits<std::int16_t>::max(),
                      std::numeric_limits<std::int16_t>::min(), 0, -1, 1});
}

BOOST_AUTO_TEST_CASE(call_int_method) {
  test_call_method<std::int32_t>(
      "intMethod", {std::numeric_limits<std::int32_t>::min(),
                    std::numeric_limits<std::int32_t>::max(), 0, -1, 1});
}

BOOST_AUTO_TEST_CASE(call_long_method) {
  test_call_method<std::int64_t>(
      "longMethod", {std::numeric_limits<std::int64_t>::min(),
                     std::numeric_limits<std::int64_t>::max(), 0, -1, 1});
}

BOOST_AUTO_TEST_CASE(call_float_method) {
  auto cls = jni::java::lang::Class::forName("MethodTest");
  auto mid = cls.getMethod<float(float)>("floatMethod");
  BOOST_REQUIRE_MESSAGE(mid != nullptr,
                        "no named named floatMethod on MethodTest");

  auto instance = cls.newInstance();

  jni::raw::method<float(float)> method{mid};

  for (const auto &value :
       {std::numeric_limits<float>::min(), std::numeric_limits<float>::max(),
        0.0f, -1.0f, 1.0f}) {
    auto result = method(jni::environment::current(), instance.ref(), value);

    BOOST_CHECK_CLOSE(result, value, 0.0001);
  }
}

BOOST_AUTO_TEST_CASE(call_double_method) {
  auto cls = jni::java::lang::Class::forName("MethodTest");
  auto mid = cls.getMethod<double(double)>("doubleMethod");
  BOOST_REQUIRE_MESSAGE(mid != nullptr,
                        "no named named doubleMethod on MethodTest");

  auto instance = cls.newInstance();

  jni::raw::method<double(double)> method{mid};

  for (const auto &value :
       {std::numeric_limits<double>::min(), std::numeric_limits<double>::max(),
        0.0, -1.0, 1.0}) {
    auto result = method(jni::environment::current(), instance.ref(), value);

    BOOST_CHECK_CLOSE(result, value, 0.0001);
  }
}

BOOST_AUTO_TEST_SUITE_END() // method_tests
BOOST_AUTO_TEST_SUITE_END() // raw_tests
