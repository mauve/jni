
#include <boost/test/unit_test.hpp>

#include <jni/java/lang/class.hpp>
#include <jni/java/lang/classloader.hpp>
#include <jni/java/lang/object.hpp>

#include <fstream>

using jni::java::lang::Class;
using jni::java::lang::ClassLoader;

std::vector<std::uint8_t> loadClassFromFile(const std::string &path) {
  std::ifstream is{ path, std::ios::binary };
  BOOST_REQUIRE_MESSAGE(is.is_open(), "cannot open: " << path);

  is.seekg(0, std::ios::end);
  int file_size = static_cast<int>(is.tellg());
  is.seekg(0, std::ios::beg);

  BOOST_TEST(file_size != 0);

  std::vector<std::uint8_t> class_buffer(static_cast<int>(file_size));
  BOOST_REQUIRE(is.read((char *)class_buffer.data(), file_size));

  return class_buffer;
}

BOOST_AUTO_TEST_SUITE(java_tests)
BOOST_AUTO_TEST_SUITE(lang_tests)
BOOST_AUTO_TEST_SUITE(classloader_tests)

BOOST_AUTO_TEST_CASE(get_system_classloader) {
  auto ldr = ClassLoader::getSystemClassLoader();

  BOOST_TEST((ldr != nullptr));
}

BOOST_AUTO_TEST_CASE(load_class) {
  auto ldr = ClassLoader::getSystemClassLoader();
  auto cls = ldr.loadClass("java.lang.String");

  BOOST_TEST((cls != nullptr));
  BOOST_TEST(cls.getName() == "java.lang.String");
}

BOOST_AUTO_TEST_CASE(define_class) {
  auto class_buffer = loadClassFromFile("..\\Debug\\DefineClass.class");

  auto ldr = ClassLoader::getSystemClassLoader();
  auto cls = ldr.defineClass("DefineClass", class_buffer.data(),
                             class_buffer.data() + class_buffer.size());

  BOOST_TEST((cls != nullptr));

  auto obj = cls.newInstance();

  BOOST_TEST((obj != nullptr));
}

BOOST_AUTO_TEST_SUITE_END() // classloader_tests
BOOST_AUTO_TEST_SUITE_END() // lang_tests
BOOST_AUTO_TEST_SUITE_END() // java_tests
