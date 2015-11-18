
#pragma once

namespace jni {

// specialize this for your Java class wrappers
// so that we can get the java class name
template <typename Class>
struct class_name {
  // should return for example java/lang/Object
  static const char * name();
};

}  // namespace jni

#define CLASS_NAME(Class, Name)                                                \
  \
namespace jni {                                                                \
    \
template<> struct class_name<Class> {                                          \
      static inline const char *name() { return Name; }                        \
    };                                                                         \
  }
