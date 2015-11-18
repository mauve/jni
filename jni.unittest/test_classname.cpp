
#include <boost/test/unit_test.hpp>

#include <jni/java/lang/class.hpp>
#include <jni/java/lang/classloader.hpp>
#include <jni/java/lang/object.hpp>
#include <jni/java/lang/throwable.hpp>

BOOST_AUTO_TEST_SUITE(classname_tests)

BOOST_AUTO_TEST_CASE(check_bundled_classes) {
  using jni::class_name;
  using jni::java::lang::Class;
  using jni::java::lang::ClassLoader;
  using jni::java::lang::Object;
  using jni::java::lang::Throwable;

  BOOST_TEST(class_name<Class>::name() == "java/lang/Class");
  BOOST_TEST(class_name<ClassLoader>::name() == "java/lang/ClassLoader");
  BOOST_TEST(class_name<Object>::name() == "java/lang/Object");
  BOOST_TEST(class_name<Throwable>::name() == "java/lang/Throwable");
}

BOOST_AUTO_TEST_SUITE_END()
