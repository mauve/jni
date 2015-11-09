
#pragma once

#include <jni/config.hpp>
#include <jni/raw/types.hpp>
#include <jni/ref.hpp>

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

  Class loadClass(const std::string &name);

  // returns a class loader which was initialized by
  // JNI_OnLoad respectively JNI_CreateJavaVM and
  // therefore is the "best" class loader
  static ClassLoader &getDefaultClassLoader();

private:
  friend class global_initialization;
  static void global_init_hook(environment &env);
};

} // namespace lang
} // namespace java
} // namespace jni
