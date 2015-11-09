
#include <boost/test/unit_test.hpp>

#include <jni/raw/signature.hpp>
#include <jni/ref.hpp>

#include <string>

template <typename F> static inline std::string g() {
  jni::raw::signature<F> s;
  return s.get();
}

using jni::raw::typed_array_ref;
using jni::raw::object_ref;
using jni::raw::class_ref;
using jni::raw::string_ref;
using jni::raw::throwable_ref;

using jni::local_ref;

BOOST_AUTO_TEST_SUITE(test_signature)

BOOST_AUTO_TEST_CASE(builtin_types_empty_args) {
  BOOST_TEST(g<void()>() == "()V");
  BOOST_TEST(g<bool()>() == "()Z");
  BOOST_TEST(g<std::uint8_t()>() == "()B");
  BOOST_TEST(g<std::uint16_t()>() == "()C");
  BOOST_TEST(g<std::int16_t()>() == "()S");
  BOOST_TEST(g<std::int32_t()>() == "()I");
  BOOST_TEST(g<std::int64_t()>() == "()J");
  BOOST_TEST(g<float()>() == "()F");
  BOOST_TEST(g<double()>() == "()D");
  BOOST_TEST(g<object_ref()>() == "()Ljava/lang/Object;");
  BOOST_TEST(g<class_ref()>() == "()Ljava/lang/Class;");
  BOOST_TEST(g<string_ref()>() == "()Ljava/lang/String;");
  BOOST_TEST(g<throwable_ref()>() == "()Ljava/lang/Throwable;");
}

BOOST_AUTO_TEST_CASE(builtin_array_types_empty_args) {
  BOOST_TEST(g<typed_array_ref<bool>()>() == "()[Z");
  BOOST_TEST(g<typed_array_ref<std::uint8_t>()>() == "()[B");
  BOOST_TEST(g<typed_array_ref<std::uint16_t>()>() == "()[C");
  BOOST_TEST(g<typed_array_ref<std::int16_t>()>() == "()[S");
  BOOST_TEST(g<typed_array_ref<std::int32_t>()>() == "()[I");
  BOOST_TEST(g<typed_array_ref<std::int64_t>()>() == "()[J");
  BOOST_TEST(g<typed_array_ref<float>()>() == "()[F");
  BOOST_TEST(g<typed_array_ref<double>()>() == "()[D");
  BOOST_TEST(g<typed_array_ref<object_ref>()>() == "()[Ljava/lang/Object;");
  BOOST_TEST(g<typed_array_ref<class_ref>()>() == "()[Ljava/lang/Class;");
  BOOST_TEST(g<typed_array_ref<string_ref>()>() == "()[Ljava/lang/String;");
  BOOST_TEST(g<typed_array_ref<throwable_ref>()>() ==
             "()[Ljava/lang/Throwable;");
}

BOOST_AUTO_TEST_CASE(builtin_types_args_return_void) {
  BOOST_TEST(g<void(bool)>() == "(Z)V");
  BOOST_TEST(g<void(std::uint8_t)>() == "(B)V");
  BOOST_TEST(g<void(std::uint16_t)>() == "(C)V");
  BOOST_TEST(g<void(std::int16_t)>() == "(S)V");
  BOOST_TEST(g<void(std::int32_t)>() == "(I)V");
  BOOST_TEST(g<void(std::int64_t)>() == "(J)V");
  BOOST_TEST(g<void(float)>() == "(F)V");
  BOOST_TEST(g<void(double)>() == "(D)V");
  BOOST_TEST(g<void(object_ref)>() == "(Ljava/lang/Object;)V");
  BOOST_TEST(g<void(class_ref)>() == "(Ljava/lang/Class;)V");
  BOOST_TEST(g<void(string_ref)>() == "(Ljava/lang/String;)V");
  BOOST_TEST(g<void(throwable_ref)>() == "(Ljava/lang/Throwable;)V");
}

BOOST_AUTO_TEST_CASE(builtin_array_types_args_return_void) {
  BOOST_TEST(g<void(typed_array_ref<bool>)>() == "([Z)V");
  BOOST_TEST(g<void(typed_array_ref<std::uint8_t>)>() == "([B)V");
  BOOST_TEST(g<void(typed_array_ref<std::uint16_t>)>() == "([C)V");
  BOOST_TEST(g<void(typed_array_ref<std::int16_t>)>() == "([S)V");
  BOOST_TEST(g<void(typed_array_ref<std::int32_t>)>() == "([I)V");
  BOOST_TEST(g<void(typed_array_ref<std::int64_t>)>() == "([J)V");
  BOOST_TEST(g<void(typed_array_ref<float>)>() == "([F)V");
  BOOST_TEST(g<void(typed_array_ref<double>)>() == "([D)V");
  BOOST_TEST(g<void(typed_array_ref<object_ref>)>() ==
             "([Ljava/lang/Object;)V");
  BOOST_TEST(g<void(typed_array_ref<class_ref>)>() == "([Ljava/lang/Class;)V");
  BOOST_TEST(g<void(typed_array_ref<string_ref>)>() ==
             "([Ljava/lang/String;)V");
  BOOST_TEST(g<void(typed_array_ref<throwable_ref>)>() ==
             "([Ljava/lang/Throwable;)V");
}

BOOST_AUTO_TEST_CASE(all_builtin_args_return_void) {
  BOOST_TEST(g<void(bool, std::uint8_t, std::uint16_t, std::int16_t,
                    std::int32_t, std::int64_t, float, double, object_ref,
                    string_ref, class_ref, throwable_ref)>() ==
             "(ZBCSIJFDLjava/lang/Object;Ljava/lang/String;Ljava/lang/"
             "Class;Ljava/lang/Throwable;)V");
}

BOOST_AUTO_TEST_SUITE_END()