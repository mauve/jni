
#include <boost/test/unit_test.hpp>

#include <jni/environment.hpp>

#include <jni/java/lang/object.hpp>

#include <sstream>
#include <string>

BOOST_AUTO_TEST_SUITE(java_object_tests)

BOOST_AUTO_TEST_CASE(create) {
  jni::local_ref<jni::raw::object_ref> ref{
      jni::environment::current().new_string("just a string")};
  jni::java::lang::Object obj{ref};

  BOOST_TEST(obj.hashCode() != 0);
}

BOOST_AUTO_TEST_CASE(toString) {
  jni::local_ref<jni::raw::object_ref> ref{
      jni::environment::current().new_string("just a string")};
  jni::java::lang::Object obj{ref};

  auto result = obj.toString();

  BOOST_TEST(result == "just a string");
}

BOOST_AUTO_TEST_CASE(stream) {
  jni::local_ref<jni::raw::object_ref> ref{
      jni::environment::current().new_string("just a string")};
  jni::java::lang::Object obj{ref};

  std::ostringstream os;
  os << obj;

  auto result = os.str();

  BOOST_TEST(result == obj.toString());
}

BOOST_AUTO_TEST_CASE(equals) {
  jni::local_ref<jni::raw::object_ref> ref1{
      jni::environment::current().new_string("just a string")};
  jni::java::lang::Object obj1{ref1};

  jni::local_ref<jni::raw::object_ref> ref2{
      jni::environment::current().new_string("just a string")};
  jni::java::lang::Object obj2{ref2};

  jni::local_ref<jni::raw::object_ref> ref3{
      jni::environment::current().new_string("a different string")};
  jni::java::lang::Object obj3{ref3};

  BOOST_TEST(obj1.equals(obj2));
  BOOST_TEST(obj2.equals(obj1));
  BOOST_TEST(!obj1.equals(obj3));
  BOOST_TEST(!obj3.equals(obj1));
  BOOST_TEST(obj1 == obj2);
  BOOST_TEST(obj2 == obj1);
  BOOST_TEST(obj1 != obj3);
  BOOST_TEST(obj3 != obj1);
}

BOOST_AUTO_TEST_SUITE_END()
