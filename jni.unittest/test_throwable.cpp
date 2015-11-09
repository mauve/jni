
#include <boost/test/unit_test.hpp>

#include <jni/java/lang/class.hpp>
#include <jni/java/lang/throwable.hpp>

#include <string>
#include <sstream>

using jni::java::lang::Class;
using jni::java::lang::Throwable;

using namespace std::literals;

BOOST_AUTO_TEST_SUITE(throwable_tests)

BOOST_AUTO_TEST_CASE(class_not_found_exception) {
  try {
    Class::forName("ThisClassShouldNotExist");
  } catch (const Throwable &throwable) {
    BOOST_TEST(throwable.what() != ""s);
  }
}

BOOST_AUTO_TEST_CASE(print_stack_trace_to_stream) {
  try {
    Class::forName("ThisClassShouldNotExist");
  } catch (const Throwable &throwable) {
    std::ostringstream os;
    BOOST_CHECK_NO_THROW(throwable.printStackTrace(os));

    auto printed = os.str();
    BOOST_TEST(printed != "");
  }
}

BOOST_AUTO_TEST_SUITE_END()