
#include <jni/java/lang/object.hpp>

#include <iostream>
#include <utility>

#include <jni/environment.hpp>
#include <jni/java/lang/class.hpp>
#include <jni/raw/method.hpp>
#include <jni/string.hpp>

namespace jni {
namespace java {
namespace lang {

struct Object::method_cache {
  raw::method<std::int32_t()> hashCode;
  raw::method<raw::string_ref()> toString;
  raw::method<bool(raw::object_ref)> equals;
  raw::method<void()> notify;
  raw::method<void()> notifyAll;
  raw::method<void()> wait1;
  raw::method<void(std::int64_t timeout)> wait2;
  raw::method<void(std::int64_t timeout, std::int32_t nanoseconds)> wait3;

  method_cache(environment &env, raw::class_ref cls)
      : hashCode{env, cls, "hashCode"}, toString{env, cls, "toString"},
        equals{env, cls, "equals"}, notify{env, cls, "notify"},
        notifyAll{env, cls, "notifyAll"}, wait1{env, cls, "wait"},
        wait2{env, cls, "wait"}, wait3{env, cls, "wait"} {}
};

Object::method_cache *Object::cache_ptr = nullptr;

void Object::global_init_hook(environment &env) {
  local_ref<raw::class_ref> cls{env.find_class("java/lang/Object")};
  static method_cache cache{env, cls.raw()};
  cache_ptr = &cache;
}

//
// Object
//

Object::Object(local_ref<raw::object_ref> &&ref) : _ref(std::move(ref)) {}
Object::Object(global_ref<raw::object_ref> &&ref) : _ref(std::move(ref)) {}

Object::Object(const Object &other) = default;
Object::Object(Object &&other) = default;

Object::~Object() = default;

int Object::hashCode() const {
  return cache_ptr->hashCode(environment::current(), _ref.raw());
}

std::string Object::toString() const {
  local_ref<raw::string_ref> result{
      cache_ptr->toString(environment::current(), _ref.raw())};

  return to_string(result.raw());
}

bool Object::equals(const Object &other) const {
  return cache_ptr->equals(environment::current(), _ref.raw(),
                           other._ref.raw());
}

Class Object::getClass() const {
  local_ref<raw::class_ref> cls{
      environment::current().get_object_class(_ref.raw())};
  return Class{cls};
}

void Object::notify() { cache_ptr->notify(environment::current(), _ref.raw()); }

void Object::notifyAll() {
  cache_ptr->notify(environment::current(), _ref.raw());
}

void Object::wait() const {
  cache_ptr->wait1(environment::current(), _ref.raw());
}

void Object::wait(const boost::chrono::milliseconds &timeout) const {
  cache_ptr->wait2(environment::current(), _ref.raw(), timeout.count());
}

void Object::wait(const boost::chrono::nanoseconds &timeout) const {
  auto milliseconds =
      boost::chrono::duration_cast<boost::chrono::milliseconds>(timeout);
  auto remainder = timeout % 1000000;

  cache_ptr->wait3(environment::current(), _ref.raw(), milliseconds.count(),
                   static_cast<std::int32_t>(remainder.count()));
}

raw::object_ref Object::ref() {
  return _ref.raw();
}

bool operator==(const Object &left, const Object &right) {
  return left.equals(right);
}

bool operator!=(const Object &left, const Object &right) {
  return !(left == right);
}

std::ostream &operator<<(std::ostream &os, const Object &object) {
  return os << object.toString();
}

//
// Monitor
//

Monitor::Monitor(Object &obj) : _obj(&obj), _entered(false) { enter(); }

Monitor::Monitor(Object &obj, defer_t) : _obj(&obj), _entered(false) {}

Monitor::Monitor(Monitor &&other) : _obj(other._obj), _entered(other._entered) {
  other._obj = nullptr;
  other._entered = false;
}

Monitor::~Monitor() { leave(); }

void Monitor::enter() {
  if (_entered || !_obj)
    return;

  environment::current().monitor_enter(_obj->_ref.raw());

  _entered = true;
}

void Monitor::leave() {
  if (!_entered || !_obj)
    return;

  environment::current().monitor_leave(_obj->_ref.raw());

  _entered = false;
}

} // namespace lang
} // namespace java
} // namespace jni
