
#include <boost/test/unit_test.hpp>

#include <jni/environment.hpp>

#include <jni/java/lang/object.hpp>

#include <sstream>
#include <string>

using jni::ref_cast;
using jni::environment;
using jni::java::lang::Object;
using jni::local_ref;
using jni::raw::object_ref;

BOOST_AUTO_TEST_SUITE(java_tests)
BOOST_AUTO_TEST_SUITE(lang_tests)
BOOST_AUTO_TEST_SUITE(object_tests)

BOOST_AUTO_TEST_CASE(create) {
  Object obj{
      ref_cast<object_ref>(environment::current().new_string("just a string"))};

  BOOST_TEST(obj.hashCode() != 0);
}

BOOST_AUTO_TEST_CASE(toString) {
  Object obj{
      ref_cast<object_ref>(environment::current().new_string("just a string"))};

  auto result = obj.toString();

  BOOST_TEST(result == "just a string");
}

BOOST_AUTO_TEST_CASE(stream) {
  Object obj{ref_cast<object_ref>(
      jni::environment::current().new_string("just a string"))};

  std::ostringstream os;
  os << obj;

  auto result = os.str();

  BOOST_TEST(result == obj.toString());
}

BOOST_AUTO_TEST_CASE(equals) {
  Object obj1{
      ref_cast<object_ref>(environment::current().new_string("just a string"))};
  Object obj2{
      ref_cast<object_ref>(environment::current().new_string("just a string"))};
  Object obj3{ref_cast<object_ref>(
      environment::current().new_string("a different string"))};

  BOOST_TEST(obj1.equals(obj2));
  BOOST_TEST(obj2.equals(obj1));
  BOOST_TEST(!obj1.equals(obj3));
  BOOST_TEST(!obj3.equals(obj1));
  BOOST_TEST(obj1 == obj2);
  BOOST_TEST(obj2 == obj1);
  BOOST_TEST(obj1 != obj3);
  BOOST_TEST(obj3 != obj1);
}

BOOST_AUTO_TEST_SUITE_END() // object_tests
BOOST_AUTO_TEST_SUITE_END() // lang_tests
BOOST_AUTO_TEST_SUITE_END() // java_tests
