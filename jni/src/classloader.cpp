
#include <jni/java/lang/classloader.hpp>

#include <jni/java/lang/class.hpp>
#include <jni/raw/method.hpp>
#include <jni/string.hpp>

namespace jni {
namespace java {
namespace lang {

struct ClassLoader::method_cache {
  global_ref<raw::class_ref> ClassLoaderClass;
  raw::method<raw::class_ref(raw::string_ref)> loadClass;

  // TODO: fix
  raw::method_id getSystemClassLoader_id;

  method_cache(environment &env, raw::class_ref cls)
      : ClassLoaderClass{cls}, loadClass{env, cls, "loadClass"} {
    getSystemClassLoader_id = env.get_static_method_id(
        cls, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");
    if (!getSystemClassLoader_id) {
      throw std::runtime_error("cannot find getSystemClassLoader method_id");
    }
  }
};

ClassLoader::method_cache *ClassLoader::cache_ptr = nullptr;
ClassLoader *ClassLoader::system_class_loader = nullptr;

void ClassLoader::global_init_hook(environment &env) {
  local_ref<raw::class_ref> cls879879{ env.find_class("MethodTest") };

  local_ref<raw::class_ref> cls{env.find_class("java/lang/ClassLoader")};
  static method_cache cache{env, cls.raw()};
  cache_ptr = &cache;

  local_ref<raw::object_ref> obj{env.call_object_method(
      cls.raw(), cache.getSystemClassLoader_id, nullptr)};
  if (!obj) {
    throw std::runtime_error("call to getSystemClassLoader failed");
  }

  static ClassLoader class_loader{std::move(obj)};
  ClassLoader::system_class_loader = &class_loader;
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
  local_ref<raw::class_ref> cls{
      cache_ptr->loadClass(environment::current(), ref(), jname.ref())};
  return Class{std::move(cls)};
}

ClassLoader &ClassLoader::getSystemClassLoader() {
  return *system_class_loader;
}

} // namespace lang
} // namespace java
} // namespace jni
