
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
  raw::method_id getSystemClassLoader;

  method_cache(Class cls)
      : defineClass{cls, "defineClass"}, loadClass{cls, "loadClass"},
        getSystemClassLoader{environment::current().get_static_method_id(
            extract_reference(cls), "getSystemClassLoader",
            "()Ljava/lang/ClassLoader;")} {}

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
  modified_utf8_string jname{name.c_str()};

  std::size_t buffer_size = end - begin;
  array<std::uint8_t> buffer{buffer_size};
  buffer.load();
  std::memcpy(buffer.data(), begin, buffer_size);
  buffer.commit();

  return Class{method_cache::get().defineClass(
      environment::current(), extract_reference(*this),
      extract_reference(jname), extract_reference(buffer), (std::int32_t)0,
      (std::int32_t)buffer_size)};
}

Class ClassLoader::loadClass(const std::string &name) {
  modified_utf8_string jname{name.c_str()};
  return Class{method_cache::get().loadClass(environment::current(),
                                             extract_reference(*this),
                                             extract_reference(jname))};
}

ClassLoader ClassLoader::getSystemClassLoader() {
  auto cls = Class::forName("java/lang/ClassLoader");

  // TODO create raw::static_method
  auto obj = environment::current().call_object_method(
      extract_reference(cls), method_cache::get().getSystemClassLoader,
      nullptr);
  throw_if_exception();
  return ClassLoader{std::move(obj)};
}

} // namespace lang
} // namespace java
} // namespace jni
