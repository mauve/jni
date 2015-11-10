
#include <boost/test/unit_test.hpp>

#include <jni/java/lang/class.hpp>
#include <jni/java/lang/throwable.hpp>

#include <string>
#include <sstream>

using jni::java::lang::Class;
using jni::java::lang::Throwable;

using namespace std::literals;

template <typename Callable> Throwable catch_throwable(Callable &&f) {
  try {
    f();
  } catch (const Throwable &ex) {
    return std::move(ex);
  } catch (const std::exception &ex) {
    BOOST_FAIL("Callable didn't throw an exception of type Throwable, instead: "
               << typeid(ex).name() << " message: " << ex.what());
    throw ex;
  }

  BOOST_FAIL("Callable didn't throw an exception at all");
  throw std::runtime_error("Expected an exception.");
}

BOOST_AUTO_TEST_SUITE(java_tests)
BOOST_AUTO_TEST_SUITE(lang_tests)
BOOST_AUTO_TEST_SUITE(throwable_tests)

BOOST_AUTO_TEST_CASE(class_not_found_exception) {
  Throwable throwable =
      catch_throwable([]() { Class::forName("ThisClassShouldNotExist"); });

  BOOST_TEST(throwable.what() != ""s);
}

BOOST_AUTO_TEST_CASE(print_stack_trace_to_stream) {
  Throwable throwable =
      catch_throwable([]() { Class::forName("ThisClassShouldNotExist"); });

  std::ostringstream os;
  BOOST_CHECK_NO_THROW(throwable.printStackTrace(os));

  auto printed = os.str();
  BOOST_TEST(printed != "");
}

BOOST_AUTO_TEST_CASE(check_real_class_name) {
  Throwable throwable =
      catch_throwable([]() { Class::forName("ThisClassShouldNotExist"); });

  auto cls = throwable.getClass();
  BOOST_TEST(cls.getName() == "java.lang.NoClassDefFoundError");
}

BOOST_AUTO_TEST_CASE(get_cause) {
  Throwable throwable =
      catch_throwable([]() { Class::forName("ThisClassShouldNotExist"); });

  auto cause = throwable.getCause();
  BOOST_TEST((cause != nullptr));
  BOOST_TEST(cause.getClass().getName() == "java.lang.ClassNotFoundException");
}

BOOST_AUTO_TEST_SUITE_END() // throwable_tests
BOOST_AUTO_TEST_SUITE_END() // lang_tests
BOOST_AUTO_TEST_SUITE_END() // java_tests