
#include <jni/java/lang/throwable.hpp>

#pragma once

#include <jni/java/lang/object.hpp>
#include <jni/java/lang/class.hpp>

#include <jni/string.hpp>
#include <jni/global_ref.hpp>
#include <jni/local_ref.hpp>
#include <jni/raw/method.hpp>
#include <jni/raw/value.hpp>

#include <array>
#include <stdexcept>
#include <string>
#include <iosfwd>

namespace jni {
namespace java {
namespace lang {

namespace {

struct method_cache {
  raw::method<void(raw::throwable_ref)> addSuppressed;
  raw::method<raw::throwable_ref()> fillInStackTrace;
  raw::method<raw::throwable_ref()> getCause;
  raw::method<raw::string_ref()> getLocalizedMessage;
  raw::method<raw::string_ref()> getMessage;
  raw::method<raw::throwable_ref(raw::throwable_ref)> initCause;
  raw::method<void()> printStackTrace;
  raw::method<void(raw::object_ref)> printStackTrace2;

  method_cache(Class cls)
      : addSuppressed{cls, "addSuppressed"},
        fillInStackTrace{cls, "fillInStackTrace"}, getCause{cls, "getCause"},
        getLocalizedMessage{cls, "getLocalizedMessage"},
        getMessage{cls, "getMessage"}, initCause{cls, "initCause"},
        printStackTrace{cls, "printStackTrace"},
        printStackTrace2{
            cls.getMethod("printStackTrace", "(Ljava/io/PrintWriter;)V")} {}

  static method_cache &get() {
    static method_cache cache{Class::forName("java/lang/Throwable")};
    return cache;
  }
};

} // namespace anonymous

Throwable::Throwable(local_ref<raw::throwable_ref> &&ref)
    : Object{ref_cast<raw::object_ref>(std::move(ref))},
      runtime_error{"Throwable"} {
  _serialized_message = getMessage();
}

Throwable::Throwable(global_ref<raw::throwable_ref> &&ref)
    : Object{ref_cast<raw::object_ref>(std::move(ref))},
      runtime_error{"Throwable"} {
  _serialized_message = getMessage();
}

Throwable::Throwable(const Throwable &other) = default;
Throwable::Throwable(Throwable &&other) = default;
Throwable::~Throwable() = default;

Throwable &Throwable::operator=(const Throwable &other) = default;
Throwable &Throwable::operator=(Throwable &&other) = default;

void Throwable::addSuppressed(const Throwable &exception) {
  method_cache::get().addSuppressed(*this, extract_reference(exception));
}

Throwable Throwable::fillInStackTrace() {
  return Throwable{method_cache::get().fillInStackTrace(*this)};
}

Throwable Throwable::getCause() const {
  return Throwable{method_cache::get().getCause(*this)};
}

std::string Throwable::getLocalizedMessage() const {
  return to_string(method_cache::get().getLocalizedMessage(*this));
}

std::string Throwable::getMessage() const {
  return to_string(method_cache::get().getMessage(*this));
}

Throwable Throwable::initCause(const Throwable &cause) {
  return Throwable{
      method_cache::get().initCause(*this, extract_reference(cause))};
}

void Throwable::printStackTrace() const {
  environment::current().clear_exceptions();
  method_cache::get().printStackTrace(*this);
}

void Throwable::printStackTrace(std::ostream &s) const {
  auto string_writer = Class::forName("java/io/StringWriter").newInstance();

  auto print_writer_class = Class::forName("java/io/PrintWriter");
  auto print_writer_constructor =
      print_writer_class.getMethod("<init>", "(Ljava/io/Writer;)V");

  std::array<raw::value, 1> constructor_args{
      raw::to_value(extract_reference(string_writer))};
  auto print_writer = environment::current().new_object(
      reinterpret_cast<raw::class_ref>(extract_reference(print_writer_class)),
      print_writer_constructor, constructor_args.data());

  method_cache::get().printStackTrace2(*this, print_writer);

  s << string_writer.toString();
}

char const *Throwable::what() const { return _serialized_message.c_str(); }

} // namespace lang

} // namespace java

} // namespace jni
