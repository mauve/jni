
#include <boost/test/unit_test.hpp>

#include <jni/java/lang/class.hpp>
#include <jni/java/lang/object.hpp>

using jni::java::lang::Class;

BOOST_AUTO_TEST_SUITE(java_class_tests)

BOOST_AUTO_TEST_CASE(get_builtin_class) {
  BOOST_CHECK_NO_THROW(auto cls = Class::forName("java/lang/String"));
}

BOOST_AUTO_TEST_CASE(create_new_instance) {
  auto cls = Class::forName("java/lang/String");
  auto obj = cls.newInstance();

  auto str = obj.toString();
  BOOST_TEST(str == "");
}

BOOST_AUTO_TEST_CASE(get_name) {
  auto cls = Class::forName("java/lang/String");

  auto name = cls.getName();
  BOOST_TEST(name == "java.lang.String");
}

BOOST_AUTO_TEST_CASE(get_simple_name) {
  auto cls = Class::forName("java/lang/String");

  auto name = cls.getSimpleName();
  BOOST_TEST(name == "String");
}

BOOST_AUTO_TEST_CASE(is_annotation) {
  auto cls = Class::forName("java/lang/String");

  BOOST_TEST(!cls.isAnnotation());
}

// need to find an annotation
//
// BOOST_AUTO_TEST_CASE(is_annotation_present) {
//  auto cls = Class::forName("java/lang/String");
//
//  BOOST_TEST(!cls.isAnnotation());
// }

BOOST_AUTO_TEST_CASE(is_anonymous_class) {
  auto cls = Class::forName("java/lang/String");

  BOOST_TEST(!cls.isAnonymousClass());
}

BOOST_AUTO_TEST_CASE(is_array) {
  auto cls = Class::forName("java/lang/String");

  BOOST_TEST(!cls.isArray());
  // TODO: test array class aswell
}

BOOST_AUTO_TEST_CASE(is_assignable_from) {
  auto cls1 = Class::forName("java/lang/String");
  auto cls2 = Class::forName("java/lang/String");

  BOOST_TEST(cls1.isAssignableFrom(cls2));
  BOOST_TEST(cls2.isAssignableFrom(cls1));
}

BOOST_AUTO_TEST_CASE(is_instance) {
  auto cls = Class::forName("java/lang/String");
  auto obj = cls.newInstance();

  BOOST_TEST(cls.isInstance(obj));
}

BOOST_AUTO_TEST_CASE(is_local_class) {
  auto cls = Class::forName("java/lang/String");

  BOOST_TEST(!cls.isLocalClass());
  // TODO: test with local class
}

BOOST_AUTO_TEST_CASE(is_member_class) {
  auto cls = Class::forName("java/lang/String");

  BOOST_TEST(!cls.isMemberClass());
}

BOOST_AUTO_TEST_CASE(is_primitive) {
  auto cls = Class::forName("java/lang/String");

  BOOST_TEST(!cls.isPrimitive());
}

BOOST_AUTO_TEST_CASE(is_synthetic) {
  auto cls = Class::forName("java/lang/String");

  BOOST_TEST(!cls.isSynthetic());
}

BOOST_AUTO_TEST_CASE(object_toString) {
  auto str = Class::forName("java/lang/String");
  auto obj = Class::forName("java/lang/Object");

  auto str_toString = str.toString();
  auto obj_toString = obj.toString();

  BOOST_TEST(str_toString != obj_toString);
}

BOOST_AUTO_TEST_CASE(object_hashCode) {
  auto str = Class::forName("java/lang/String");
  auto obj = Class::forName("java/lang/Object");

  auto str_hashCode = str.hashCode();
  auto obj_hashCode = obj.hashCode();

  BOOST_TEST(str_hashCode != obj_hashCode);
}

BOOST_AUTO_TEST_CASE(object_equals) {
  auto str = Class::forName("java/lang/String");
  auto str_copy = Class::forName("java/lang/String");
  auto obj = Class::forName("java/lang/Object");

  BOOST_TEST(!str.equals(obj));
  BOOST_TEST(str.equals(str_copy));
}

BOOST_AUTO_TEST_SUITE_END()