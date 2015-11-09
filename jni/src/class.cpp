
#include <jni/java/lang/class.hpp>

#include <utility>

#include <jni/environment.hpp>
#include <jni/java/lang/classloader.hpp>
#include <jni/java/lang/object.hpp>
#include <jni/raw/method.hpp>

namespace jni {
namespace java {
namespace lang {

namespace {

struct method_cache {
  raw::method<local_ref<raw::object_ref>()> newInstance;
  raw::method<local_ref<raw::object_ref>()> getClassLoader;

  method_cache(Class cls)
      : newInstance{cls, "newInstance"},
        getClassLoader{
            cls.getMethod("getClassLoader", "()Ljava/lang/ClassLoader;")} {}

  static method_cache &get() {
    static method_cache cache{Class::forName("java/lang/Class")};
    return cache;
  }
};

} // namespace anonymous

Class::Class(local_ref<raw::class_ref> &&cls) : _ref(std::move(cls)) {}
Class::Class(global_ref<raw::class_ref> &&cls) : _ref(std::move(cls)) {}
Class::Class(const Class &other) = default;
Class::Class(Class &&other) = default;
Class::~Class() = default;

Class &Class::operator=(const Class &other) = default;
Class &Class::operator=(Class &&other) = default;

Class Class::forName(const char *name) {
  global_ref<raw::class_ref> cls{environment::current().find_class(name)};
  if (!cls) {
    // TODO: throw error
    throw std::runtime_error("class not found");
  }

  return Class{std::move(cls)};
}

raw::method_id Class::getMethod(const char *name, const char *signature) {
  return environment::current().get_method_id(_ref.raw(), name, signature);
}

ClassLoader Class::getClassLoader() const {
  return ClassLoader{
      method_cache::get().getClassLoader(environment::current(), _ref.raw())};
}

Object Class::newInstance() const {
  return Object{
      method_cache::get().newInstance(environment::current(), _ref.raw())};
}

} // namespace lang
} // namespace java
} // namespace jni
