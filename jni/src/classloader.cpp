
#include <jni/java/lang/classloader.hpp>

#include <jni/java/lang/class.hpp>
#include <jni/raw/method.hpp>
#include <jni/string.hpp>

namespace jni {
namespace java {
namespace lang {

namespace {

struct method_cache {
  raw::method<local_ref<raw::class_ref>(raw::string_ref)> loadClass;

  method_cache(Class cls) : loadClass{cls, "loadClass"} {}

  static method_cache &get() {
    static method_cache cache{Class::forName("java/lang/ClassLoader")};
    return cache;
  }
};

} // anonymous

void ClassLoader::global_init_hook(environment &env) {
  // TODO: impl
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
  return Class{method_cache::get().loadClass(environment::current(), ref(),
                                             jname.ref())};
}

} // namespace lang
} // namespace java
} // namespace jni
