
#include <jni/java/lang/class.hpp>

#include <utility>

#include <jni/environment.hpp>
#include <jni/java/lang/object.hpp>
#include <jni/raw/method.hpp>

namespace jni {
namespace java {
namespace lang {

struct Class::method_cache {
  raw::method<raw::object_ref()> newInstance;

  method_cache(environment &env, raw::class_ref cls)
      : newInstance{env, cls, "newInstance"} {}
};

Class::method_cache *Class::cache_ptr = nullptr;

void Class::global_init_hook(environment &env) {
  local_ref<raw::class_ref> cls{env.find_class("java/lang/Class")};
  static method_cache cache{env, cls.raw()};
  cache_ptr = &cache;
}

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

Object Class::newInstance() const {
  local_ref<raw::object_ref> obj{
      cache_ptr->newInstance(environment::current(), _ref.raw())};
  return Object{std::move(obj)};
}

} // namespace lang
} // namespace java
} // namespace jni
