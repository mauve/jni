
#pragma once

#include <jni/config.hpp>
#include <jni/raw/signature.hpp>
#include <jni/raw/types.hpp>
#include <jni/ref.hpp>

namespace jni {
namespace java {
namespace lang {

class Object;
class ClassLoader;

class JNI_EXPORT Class final {
public:
  explicit Class(local_ref<raw::class_ref> &&cls);
  explicit Class(global_ref<raw::class_ref> &&cls);
  Class(const Class &other);
  Class(Class &&other);
  ~Class();

  Class &operator=(const Class &other);
  Class &operator=(Class &&other);

  static Class forName(const char *name);

  raw::method_id getMethod(const char *name, const char *signature);

  template <typename Function> raw::method_id getMethod(const char *name);

  ClassLoader getClassLoader() const;

  Object newInstance() const;

private:

  global_ref<raw::class_ref> _ref;
};

template <typename Function>
inline raw::method_id Class::getMethod(const char *name) {
  raw::signature<Function> signature;

  return this->getMethod(name, signature.get());
}

} // namespace lang
} // namespace java
} // namespace jni
