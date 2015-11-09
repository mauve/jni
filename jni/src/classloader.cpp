
#include <jni/java/lang/classloader.hpp>

#include <jni/java/lang/class.hpp>
#include <jni/raw/method.hpp>
#include <jni/string.hpp>

namespace jni {
namespace java {
namespace lang {

namespace {

struct method_cache {
  global_ref<raw::class_ref> ClassLoader;
  raw::method<local_ref<raw::class_ref>(raw::string_ref)> loadClass;

  method_cache()
      : ClassLoader{environment::current().find_class("java/lang/ClassLoader")},
        loadClass{environment::current(), ClassLoader.raw(), "loadClass"} {}

  static method_cache &get() {
    static method_cache cache;
    return cache;
  }
};

} // anonymous

static ClassLoader *default_class_loader = nullptr;

void ClassLoader::global_init_hook(environment &env) {
  // get the best classloader, which happens to be the classloader
  // associated with any object AS LONG as we are getting the
  // classloader during the JNI_OnLoad or CreateVM call,
  // which is when we are calling this method

  // TODO: fix
  auto cls = env.find_class("java/lang/Class");
  raw::method_id getClassLoader{ env.get_method_id(cls.raw(), "getClassLoader", "()Ljava/lang/ClassLoader;") };

  auto str = env.new_string("");
  auto strcls = env.get_object_class(str.raw());

  auto ldr = env.call_object_method(strcls.raw(), getClassLoader, nullptr);

  if (!ldr) {
    // getClassLoader can return null instead of an instance of the bootstrap
    // loader, but because we need an instance we call getSystemClassLoader
    // instead
    auto cls = env.find_class("java/lang/ClassLoader");
    raw::method_id getSystemClassLoader{ env.get_static_method_id(cls.raw(), "getSystemClassLoader", "()Ljava/lang/ClassLoader;") };

    ldr = std::move(env.call_object_method(cls.raw(), getSystemClassLoader, nullptr));
  }

  static ClassLoader default_class_loader_instance{std::move(ldr)};
  default_class_loader = &default_class_loader_instance;
}

ClassLoader::ClassLoader(local_ref<raw::object_ref> &&ref)
    : Object(std::move(ref)) {}
ClassLoader::ClassLoader(global_ref<raw::object_ref> &&ref)
    : Object(std::move(ref)) {}

ClassLoader::ClassLoader(const ClassLoader &) = default;
ClassLoader::ClassLoader(ClassLoader &&) = default;

ClassLoader::~ClassLoader() = default;

Class ClassLoader::loadClass(const std::string &name) {
  modified_utf8_string jname{name.c_str()};
  return Class{ method_cache::get().loadClass(
    environment::current(), ref(), jname.ref()) };
}

// ClassLoader &ClassLoader::getSystemClassLoader() {
//  return *system_class_loader;
//}

ClassLoader &ClassLoader::getDefaultClassLoader() {
  if (!default_class_loader) {
    // TODO better error
    throw std::runtime_error(
        "init error, global init hooks missing for ClassLoader");
  }
  return *default_class_loader;
}

} // namespace lang
} // namespace java
} // namespace jni
