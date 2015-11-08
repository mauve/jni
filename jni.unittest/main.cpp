
#define BOOST_TEST_MODULE jni-unittest
#include <boost/test/unit_test.hpp>

#include <jni/vm.hpp>

struct embed_vm_fixture {
  embed_vm_fixture() {
    auto args = jni::vm::get_default_init_args();
    args.options.push_back(std::make_pair("-Djava.class.path=C:\\Users\\Mikael\\DEV\\other-repositories\\jni\\Debug\\MethodTest.jar", nullptr));

    auto result = jni::vm::create_vm(args);
    if (result) {
      throw std::runtime_error("could not setup vm: " + result.message());
    }
  }

  ~embed_vm_fixture() {
    auto result = jni::vm::current().destroy();
  }
};

BOOST_GLOBAL_FIXTURE(embed_vm_fixture);
