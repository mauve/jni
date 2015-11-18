
#include <jni/java/lang/class.hpp>

#include <utility>

#include <jni/environment.hpp>
#include <jni/java/lang/classloader.hpp>
#include <jni/java/lang/object.hpp>
#include <jni/raw/method.hpp>
#include <jni/string.hpp>
#include <jni/detail/throw_if_exception.hpp>

namespace jni {
namespace java {
namespace lang {

namespace {

struct method_cache {
  raw::method<local_ref<raw::object_ref>()> newInstance;
  raw::method<local_ref<raw::object_ref>()> getClassLoader;
  raw::method<local_ref<raw::string_ref>()> getName;
  raw::method<local_ref<raw::string_ref>()> getSimpleName;
  raw::method<bool()> isAnnotation;
  raw::method<bool(raw::class_ref)> isAnnotationPresent;
  raw::method<bool()> isAnonymousClass;
  raw::method<bool()> isArray;
  raw::method<bool(raw::class_ref)> isAssignableFrom;
  raw::method<bool()> isEnum;
  raw::method<bool(raw::object_ref)> isInstance;
  raw::method<bool()> isInterface;
  raw::method<bool()> isLocalClass;
  raw::method<bool()> isMemberClass;
  raw::method<bool()> isPrimitive;
  raw::method<bool()> isSynthetic;

  method_cache(Class cls)
      : newInstance{cls, "newInstance"},
        getClassLoader{cls, "getClassLoader", "()Ljava/lang/ClassLoader;"},
        getName{cls, "getName"}, getSimpleName{cls, "getSimpleName"},
        isAnnotation{cls, "isAnnotation"},
        isAnnotationPresent{cls, "isAnnotationPresent"},
        isAnonymousClass{cls, "isAnonymousClass"}, isArray{cls, "isArray"},
        isAssignableFrom{cls, "isAssignableFrom"}, isEnum{cls, "isEnum"},
        isInstance{cls, "isInstance"}, isInterface{cls, "isInterface"},
        isLocalClass{cls, "isLocalClass"}, isMemberClass{cls, "isMemberClass"},
        isPrimitive{cls, "isPrimitive"}, isSynthetic{cls, "isSynthetic"} {}

  static method_cache &get() {
    static method_cache cache{Class::forName("java/lang/Class")};
    return cache;
  }
};

} // namespace anonymous

Class::Class(local_ref<raw::class_ref> &&cls)
    : Object(ref_cast<raw::object_ref>(std::move(cls))) {}
Class::Class(global_ref<raw::class_ref> &&cls)
    : Object(ref_cast<raw::object_ref>(std::move(cls))) {}
Class::Class(const Class &other) = default;
Class::Class(Class &&other) = default;
Class::~Class() = default;

Class &Class::operator=(const Class &other) = default;
Class &Class::operator=(Class &&other) = default;

Class Class::forName(const char *name) {
  global_ref<raw::class_ref> cls{environment::current().find_class(name)};
  throw_if_exception();
  return Class{std::move(cls)};
}

raw::method_id Class::getMethod(const char *name, const char *signature) {
  auto result = environment::current().get_method_id(extract_reference(*this),
                                                     name, signature);
  throw_if_exception();
  return result;
}

raw::method_id Class::getStaticMethod(const char *name, const char *signature) {
  auto result = environment::current().get_static_method_id(
      extract_reference(*this), name, signature);
  throw_if_exception();
  return result;
}

ClassLoader Class::getClassLoader() const {
  return ClassLoader{method_cache::get().getClassLoader(*this)};
}

Object Class::newInstance() const {
  return Object{method_cache::get().newInstance(*this)};
}

std::string Class::getName() const {
  return to_string(method_cache::get().getName(*this));
}

std::string Class::getSimpleName() const {
  return to_string(method_cache::get().getSimpleName(*this));
}

// Returns true if this Class object represents an annotation type.
bool Class::isAnnotation() const {
  return method_cache::get().isAnnotation(*this);
}

// Returns true if an annotation for the specified type is present on this
// element, else false.
bool Class::isAnnotationPresent(const Class &annotationClass) {
  return method_cache::get().isAnnotationPresent(
      *this, extract_reference(annotationClass));
}

// Returns true if and only if the underlying class is an anonymous class.
bool Class::isAnonymousClass() const {
  return method_cache::get().isAnonymousClass(*this);
}

// Determines if this Class object represents an array class.
bool Class::isArray() const { return method_cache::get().isArray(*this); }

// Determines if the class or interface represented by this Class object is
// either the same as, or is a superclass or superinterface of, the class or
// interface represented by the specified Class parameter.
bool Class::isAssignableFrom(const Class &cls) const {
  return method_cache::get().isAssignableFrom(*this, extract_reference(cls));
}

// Returns true if and only if this class was declared as an enum in the source
// code.
bool Class::isEnum() const { return method_cache::get().isEnum(*this); }

// Determines if the specified Object is assignment - compatible with the object
// represented by this Class.
bool Class::isInstance(const Object &obj) const {
  return method_cache::get().isInstance(*this, extract_reference(obj));
}

// Determines if the specified Class object represents an interface type.
bool Class::isInterface() const {
  return method_cache::get().isInterface(*this);
}

// Returns true if and only if the underlying class is a local class.
bool Class::isLocalClass() const {
  return method_cache::get().isLocalClass(*this);
}

// Returns true if and only if the underlying class is a member class.
bool Class::isMemberClass() const {
  return method_cache::get().isMemberClass(*this);
}

// Determines if the specified Class object represents a primitive type.
bool Class::isPrimitive() const {
  return method_cache::get().isPrimitive(*this);
}

// Returns true if this class is a synthetic class; returns false otherwise.
bool Class::isSynthetic() const {
  return method_cache::get().isSynthetic(*this);
}

raw::class_ref extract_reference(const Class &cls) {
  return reinterpret_cast<raw::class_ref>(
      extract_reference(static_cast<const Object &>(cls)));
}

} // namespace lang
} // namespace java
} // namespace jni
