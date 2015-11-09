
#include <jni/init.hpp>

#include <jni/environment.hpp>
#include <jni/java/lang/class.hpp>
#include <jni/java/lang/object.hpp>
#include <jni/java/lang/classloader.hpp>
#include <jni/vm.hpp>

#include <jni.h>

namespace jni {

class global_initialization {
public:
  template <typename T> static void invoke_hook(environment &env) {
    T::global_init_hook(env);
  }
};

namespace init {

void run_init_hooks(environment &env) {
  global_initialization::invoke_hook<java::lang::ClassLoader>(env);
}

api_version on_load(void *_vm) {

  JavaVM *vm = reinterpret_cast<JavaVM *>(_vm);

  vm::set_vm(vm);
  environment &main_environment = environment::current();

  run_init_hooks(main_environment);

  return api_version::v1_4;
}

} // namespace init
} // namespace jni
