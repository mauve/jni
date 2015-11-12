
#pragma once

#include <jni/config.hpp>
#include <jni/raw/signature.hpp>
#include <jni/raw/types.hpp>
#include <jni/global_ref.hpp>
#include <jni/local_ref.hpp>

#include <jni/java/lang/object.hpp>

namespace jni {
namespace java {
namespace lang {

class Object;
class ClassLoader;

class JNI_EXPORT Class final : public Object {
public:
  explicit Class(local_ref<raw::class_ref> &&cls);
  explicit Class(global_ref<raw::class_ref> &&cls);
  Class(const Class &other);
  Class(Class &&other);
  virtual ~Class();

  Class &operator=(const Class &other);
  Class &operator=(Class &&other);

  static Class forName(const char *name);

  raw::method_id getMethod(const char *name, const char *signature);

  template <typename Function> raw::method_id getMethod(const char *name);

  // Returns the class loader for the class.
  ClassLoader getClassLoader() const;

  // Creates a new instance of the class represented by this Class object.
  Object newInstance() const;

  // Returns the name of the entity(class, interface, array class, primitive
  // type, or void) represented by this Class object, as a String.
  // For example: java.lang.String
  std::string getName() const;

  // Returns the simple name of the underlying class as given in the source
  // code.
  // For example: String
  std::string getSimpleName() const;

  // Returns true if this Class object represents an annotation type.
  bool isAnnotation() const;

  // Returns true if an annotation for the specified type is present on this
  // element, else false.
  bool isAnnotationPresent(const Class &annotationClass);

  // Returns true if and only if the underlying class is an anonymous class.
  bool isAnonymousClass() const;

  // Determines if this Class object represents an array class.
  bool isArray() const;

  // Determines if the class or interface represented by this Class object is
  // either the same as, or is a superclass or superinterface of, the class or
  // interface represented by the specified Class parameter.
  bool isAssignableFrom(const Class &cls) const;

  // Returns true if and only if this class was declared as an enum in the
  // source code.
  bool isEnum() const;

  // Determines if the specified Object is assignment - compatible with the
  // object represented by this Class.
  bool isInstance(const Object &obj) const;

  // Determines if the specified Class object represents an interface type.
  bool isInterface() const;

  // Returns true if and only if the underlying class is a local class.
  bool isLocalClass() const;

  // Returns true if and only if the underlying class is a member class.
  bool isMemberClass() const;

  // Determines if the specified Class object represents a primitive type.
  bool isPrimitive() const;

  // Returns true if this class is a synthetic class; returns false otherwise.
  bool isSynthetic() const;
};

template <typename Function>
inline raw::method_id Class::getMethod(const char *name) {
  raw::signature<Function> signature;

  return this->getMethod(name, signature.get());
}

raw::class_ref extract_reference(const Class &cls);

} // namespace lang
} // namespace java
} // namespace jni
