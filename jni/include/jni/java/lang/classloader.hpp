
#pragma once

#include <jni/config.hpp>
#include <jni/class_name.hpp>
#include <jni/raw/types.hpp>
#include <jni/global_ref.hpp>
#include <jni/local_ref.hpp>
#include <jni/environment.hpp>

#include <jni/java/lang/object.hpp>

namespace jni {

class global_initialization;

namespace java {
namespace lang {

class JNI_EXPORT ClassLoader : public Object {
public:
  explicit ClassLoader(local_ref<raw::object_ref> &&ref);
  explicit ClassLoader(global_ref<raw::object_ref> &&ref);

  ClassLoader(const ClassLoader &other);
  ClassLoader(ClassLoader &&other);
  virtual ~ClassLoader();

  Class defineClass(const std::string &name, const std::uint8_t *begin,
                    const std::uint8_t *end);
  Class loadClass(const std::string &name);

  static ClassLoader getSystemClassLoader();

private:
  friend class global_initialization;
  static void global_init_hook(environment &env);
};

} // namespace lang
} // namespace java
} // namespace jni

CLASS_NAME(jni::java::lang::ClassLoader, "java/lang/ClassLoader");
