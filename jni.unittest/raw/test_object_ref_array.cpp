
#include <boost/test/unit_test.hpp>

#include <jni/raw/object_ref_array.hpp>
#include <jni/raw/method.hpp>

#include <jni/java/lang/class.hpp>

using jni::java::lang::Class;
using jni::raw::method;
using jni::raw::typed_array_ref;
using jni::raw::string_ref;

jni::raw::object_ref_array get_array() {
  auto cls = Class::forName("ObjectRefArray");
  auto obj = cls.newInstance();

  method<typed_array_ref<string_ref>()> getStringArray{cls, "getStringArray"};

  auto arr = getStringArray(jni::environment::current(), extract_reference(obj));

  BOOST_TEST((arr != nullptr));

  return jni::raw::object_ref_array{std::move(arr)};
}

BOOST_AUTO_TEST_SUITE(raw_tests)
BOOST_AUTO_TEST_SUITE(object_ref_array_tests)

BOOST_AUTO_TEST_CASE(create_from_reference) {
  auto array = get_array();

  BOOST_TEST(array.size() == 2);
}

BOOST_AUTO_TEST_CASE(get_item_at_index) {
  auto array = get_array();

  BOOST_TEST(array.size() == 2);

  jni::java::lang::Object first{array.get(0)};
  jni::java::lang::Object last{array.get(1)};

  BOOST_TEST(first.getClass().getSimpleName() == "String");
  BOOST_TEST(last.getClass().getSimpleName() == "String");

  BOOST_TEST(first.toString() == "First");
  BOOST_TEST(last.toString() == "Last");

  BOOST_CHECK_THROW(array.get(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(set_item_at_index) {
  auto array = get_array();

  auto new_string = jni::environment::current().new_string("NewString");
  array.set(1, new_string.raw());

  BOOST_CHECK_THROW(array.set(2, new_string.raw()), std::out_of_range);

  auto cls = Class::forName("ObjectRefArray");
  auto obj = cls.newInstance();

  method<void(typed_array_ref<string_ref>)> testStringArray{cls,
                                                            "testStringArray"};

  BOOST_CHECK_NO_THROW(
      testStringArray(jni::environment::current(), extract_reference(obj), array.ref()));
}

BOOST_AUTO_TEST_SUITE_END() // object_ref_array_tests
BOOST_AUTO_TEST_SUITE_END() // raw
