
#pragma once

#include <jni/config.hpp>
#include <jni/class_name.hpp>
#include <jni/raw/types.hpp>
#include <jni/global_ref.hpp>
#include <jni/local_ref.hpp>

#include <boost/chrono/duration.hpp>

#include <string>
#include <iosfwd>

namespace jni {
namespace java {
namespace lang {

class Class;
class Monitor;

class JNI_EXPORT Object {
public:
  explicit Object(local_ref<raw::object_ref> &&ref);
  explicit Object(global_ref<raw::object_ref> &&ref);

  Object(const Object &other);
  Object(Object &&other);
  virtual ~Object();

  Object &operator=(const Object &obj);
  Object &operator=(Object &&obj);

  int hashCode() const;
  std::string toString() const;
  bool equals(const Object &other) const;

  Class getClass() const;

  void notify();
  void notifyAll();
  void wait() const;
  void wait(const boost::chrono::milliseconds &timeout) const;
  void wait(const boost::chrono::nanoseconds &timeout) const;

  friend JNI_EXPORT raw::object_ref extract_reference(const Object& obj);

  template <typename Target>
  friend Target object_cast(const Object &obj);

private:
  friend class Monitor;

  global_ref<raw::object_ref> _ref;
};


JNI_EXPORT bool operator==(const Object &left, const Object &right);
JNI_EXPORT bool operator!=(const Object &left, const Object &right);
JNI_EXPORT bool operator==(const Object &left, std::nullptr_t);
JNI_EXPORT bool operator==(std::nullptr_t, const Object &right);
JNI_EXPORT bool operator!=(const Object &left, std::nullptr_t);
JNI_EXPORT bool operator!=(std::nullptr_t, const Object &right);
JNI_EXPORT std::ostream &operator<<(std::ostream &os, const Object &object);

class JNI_EXPORT Monitor final {
public:
  static const struct defer_t {
  } DeferEntry;

  explicit Monitor(Object &obj);
  Monitor(Object &obj, defer_t);
  Monitor(Monitor &&other);
  ~Monitor();

  void enter();
  void leave();

  Monitor(const Monitor &other) = delete;
  Monitor &operator=(const Monitor &other) = delete;

private:
  Object *_obj;
  bool _entered;
};

} // namespace lang
} // namespace java
} // namespace jni

CLASS_NAME(jni::java::lang::Object, "java/lang/Object");
