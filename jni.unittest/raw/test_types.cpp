
#include <boost/test/unit_test.hpp>

#include <jni/raw/types.hpp>

#include <string>

using namespace std::literals;
using jni::raw::get_signature;

BOOST_AUTO_TEST_SUITE(raw_tests)
BOOST_AUTO_TEST_SUITE(types_tests)

BOOST_AUTO_TEST_CASE(verify_signatures) {
  BOOST_TEST(get_signature<bool>::get() == "Z"s);
  BOOST_TEST(get_signature<std::uint8_t>::get() == "B"s);
  BOOST_TEST(get_signature<std::uint16_t>::get() == "C"s);
  BOOST_TEST(get_signature<std::int16_t>::get() == "S"s);
  BOOST_TEST(get_signature<std::int32_t>::get() == "I"s);
  BOOST_TEST(get_signature<std::int64_t>::get() == "J"s);
  BOOST_TEST(get_signature<float>::get() == "F"s);
  BOOST_TEST(get_signature<double>::get() == "D"s);
  BOOST_TEST(get_signature<void>::get() == "V"s);
  BOOST_TEST(get_signature<jni::raw::object_ref>::get() ==
             "Ljava/lang/Object;"s);
  BOOST_TEST(get_signature<jni::raw::string_ref>::get() ==
             "Ljava/lang/String;"s);
  BOOST_TEST(get_signature<jni::raw::class_ref>::get() == "Ljava/lang/Class;"s);
  BOOST_TEST(get_signature<jni::raw::throwable_ref>::get() ==
             "Ljava/lang/Throwable;"s);

  BOOST_TEST(get_signature<jni::raw::typed_array_ref<bool>>::get() == "[Z"s);
  BOOST_TEST(get_signature<jni::raw::typed_array_ref<std::uint8_t>>::get() ==
             "[B"s);
  BOOST_TEST(get_signature<jni::raw::typed_array_ref<std::uint16_t>>::get() ==
             "[C"s);
  BOOST_TEST(get_signature<jni::raw::typed_array_ref<std::int16_t>>::get() ==
             "[S"s);
  BOOST_TEST(get_signature<jni::raw::typed_array_ref<std::int32_t>>::get() ==
             "[I"s);
  BOOST_TEST(get_signature<jni::raw::typed_array_ref<std::int64_t>>::get() ==
             "[J"s);
  BOOST_TEST(get_signature<jni::raw::typed_array_ref<float>>::get() == "[F"s);
  BOOST_TEST(get_signature<jni::raw::typed_array_ref<double>>::get() == "[D"s);
  BOOST_TEST(
      get_signature<jni::raw::typed_array_ref<jni::raw::object_ref>>::get() ==
      "[Ljava/lang/Object;"s);
  BOOST_TEST(
      get_signature<jni::raw::typed_array_ref<jni::raw::string_ref>>::get() ==
      "[Ljava/lang/String;"s);
  BOOST_TEST(
      get_signature<jni::raw::typed_array_ref<jni::raw::class_ref>>::get() ==
      "[Ljava/lang/Class;"s);
  BOOST_TEST(get_signature<
                 jni::raw::typed_array_ref<jni::raw::throwable_ref>>::get() ==
             "[Ljava/lang/Throwable;"s);
}

BOOST_AUTO_TEST_SUITE_END() // types_tests
BOOST_AUTO_TEST_SUITE_END() // raw_tests
