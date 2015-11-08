
#pragma once

#include <jni/vm.hpp>
#include <jni/errors.hpp>
#include <jni/environment.hpp>

#include <jni.h>

#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_factories.hpp>

#include <cstring>

using boost::system::error_code;

namespace jni {

namespace init {

void run_init_hooks(environment &env);

} // namespace init

namespace impl {

class jni_vm : public vm {
public:
  jni_vm(JavaVM *java_vm) : _vm{java_vm}, _created{false} {}

  ~jni_vm() {
    if (_created) {
      destroy();
    }
  }

  void attach(JavaVM *java_vm, bool created) {
    destroy();

    auto lock = boost::make_unique_lock(_mutex);
    _vm = java_vm;
    _created = created;
  }

  error_code destroy() override {
    if (!_created || !_vm)
      return error_codes::success;

    auto lock = boost::make_unique_lock(_mutex);
    auto result = static_cast<error_codes>(_vm->DestroyJavaVM());
    _vm = nullptr;
    return result;
  }

  error_code attach_current_thread_impl(const std::string &thread_name,
                                        api_version version,
                                        raw::object_ref group, bool as_daemon,
                                        void **out_env) {
    auto lock = boost::make_unique_lock(_mutex);
    if (!_vm)
      return error_codes::vm_destroyed;

    JavaVMAttachArgs args;
    args.version = static_cast<jint>(version);
    args.name = const_cast<char *>(thread_name.c_str());
    args.group = reinterpret_cast<jobject>(group);

    JNIEnv *env = nullptr;
    error_codes result;
    if (as_daemon) {
      result = static_cast<error_codes>(_vm->AttachCurrentThreadAsDaemon(
          reinterpret_cast<void **>(&env), reinterpret_cast<void *>(&args)));
      if (out_env)
        *out_env = env;
    } else {
      result = static_cast<error_codes>(_vm->AttachCurrentThread(
          reinterpret_cast<void **>(&env), reinterpret_cast<void *>(&args)));
      if (out_env)
        *out_env = env;
    }

    return result;
  }

  error_code attach_current_thread(const std::string &thread_name,
                                   api_version version, raw::object_ref group,
                                   void **env) override {
    return attach_current_thread_impl(thread_name, version, group, false, env);
  }

  error_code attach_current_thread_as_daemon(const std::string &thread_name,
                                             api_version version,
                                             raw::object_ref group,
                                             void **env) override {
    return attach_current_thread_impl(thread_name, version, group, true, env);
  }

  error_code detach_current_thread() override {
    auto lock = boost::make_unique_lock(_mutex);
    if (!_vm)
      return error_codes::vm_destroyed;
    return static_cast<error_codes>(_vm->DetachCurrentThread());
  }

  error_code get_current_env(api_version version, void **env) override {
    return static_cast<error_codes>(
        _vm->GetEnv(env, static_cast<jint>(version)));
  }

private:
  boost::mutex _mutex;
  JavaVM *_vm;
  bool _created;
};

static jni_vm global_vm{nullptr};

} // namespace impl

vm::~vm() {}

vm &vm::current() { return impl::global_vm; }

vm::init_args vm::get_default_init_args() {
  JavaVMInitArgs jni_args = { 0 };
  jni_args.version = static_cast<jint>(api_version::v1_4);
  auto result = static_cast<error_codes>(
      JNI_GetDefaultJavaVMInitArgs(reinterpret_cast<void *>(&jni_args)));
  if (result != error_codes::success) {
    throw boost::system::system_error(result, "get_default_init_args failed");
  }

  init_args args;
  args.version = static_cast<api_version>(jni_args.version);
  args.ignore_unrecognized = !!jni_args.ignoreUnrecognized;

  for (int i = 0; i < jni_args.nOptions; i++) {
    args.options.push_back(std::make_pair(jni_args.options[i].optionString,
                                          jni_args.options[i].extraInfo));
  }

/*  args.options.push_back(std::make_pair("-verbose", nullptr));
  args.options.push_back(std::make_pair("-verbose:jni", nullptr));
  args.options.push_back(std::make_pair("-Djava.library.path=C:\\Users\\Mikael\\.vulcan\\cache\\jdk1.8.0_60-win32.zip-ef26bd8bc0fd88a0b7ebe8eb8f7328aa8d9bf9c4\\extracted\\jdk\\jre\\bin", nullptr));
*/
  return args;
}

error_code
vm::create_vm(const init_args &args /* = get_default_init_args() */) {
  JavaVMInitArgs jni_args;
  jni_args.ignoreUnrecognized = args.ignore_unrecognized;
  jni_args.version = static_cast<jint>(args.version);

  std::vector<JavaVMOption> options;
  for (const auto& opt : args.options) {
    JavaVMOption option;
    option.optionString = _strdup(opt.first.c_str());
    option.extraInfo = opt.second;
    options.push_back(option);
  }
  jni_args.options = options.data();
  jni_args.nOptions = options.size();

  JavaVM *vm = nullptr;
  JNIEnv *env = nullptr;
  auto result = static_cast<error_codes>(
      JNI_CreateJavaVM(&vm, reinterpret_cast<void **>(&env),
                       reinterpret_cast<void *>(&jni_args)));

  // cleanup opts
  for (auto i = 0; i < jni_args.nOptions; ++i) {
    free(jni_args.options[i].optionString);
  }

  if (result == error_codes::success) {
    impl::global_vm.attach(vm, true);
    init::run_init_hooks(environment::current());
  }
  return result;
}

void vm::set_vm(void *vm) {
  impl::global_vm.attach(reinterpret_cast<JavaVM *>(vm), false);
}

} // namespace jni
