
#include <jni/java/lang/classloader.hpp>

#include <jni/java/lang/class.hpp>
#include <jni/raw/method.hpp>
#include <jni/array.hpp>
#include <jni/string.hpp>

namespace jni {
namespace java {
namespace lang {

namespace {

struct method_cache {
  raw::method<local_ref<raw::class_ref>(
      raw::string_ref, raw::typed_array_ref<std::uint8_t>, std::int32_t,
      std::int32_t)> defineClass;
  raw::method<local_ref<raw::class_ref>(raw::string_ref)> loadClass;
  raw::static_method<local_ref<raw::object_ref>()> getSystemClassLoader;

  method_cache(Class cls)
      : defineClass{cls, "defineClass"}, loadClass{cls, "loadClass"},
        getSystemClassLoader{cls, "getSystemClassLoader",
            "()Ljava/lang/ClassLoader;"} {}

  static method_cache &get() {
    static method_cache cache{Class::forName("java/lang/ClassLoader")};
    return cache;
  }
};

} // anonymous

void ClassLoader::global_init_hook(environment &env) {
  // force init
  method_cache::get();
}

ClassLoader::ClassLoader(local_ref<raw::object_ref> &&ref)
    : Object(std::move(ref)) {}
ClassLoader::ClassLoader(global_ref<raw::object_ref> &&ref)
    : Object(std::move(ref)) {}

ClassLoader::ClassLoader(const ClassLoader &) = default;
ClassLoader::ClassLoader(ClassLoader &&) = default;

ClassLoader::~ClassLoader() = default;

Class ClassLoader::defineClass(const std::string &name,
                               const std::uint8_t *begin,
                               const std::uint8_t *end) {
  string jname{name.c_str()};

  array<std::uint8_t> buffer{begin, end};

  return Class{method_cache::get().defineClass(
      *this, extract_reference(jname), extract_reference(buffer),
      (std::int32_t)0, (std::int32_t)buffer.size())};
}

Class ClassLoader::loadClass(const std::string &name) {
  string jname{name.c_str()};
  return Class{method_cache::get().loadClass(*this, extract_reference(jname))};
}

ClassLoader ClassLoader::getSystemClassLoader() {
  auto cls = Class::forName("java/lang/ClassLoader");
  return ClassLoader{method_cache::get().getSystemClassLoader(cls)};
}

} // namespace lang
} // namespace java
} // namespace jni
