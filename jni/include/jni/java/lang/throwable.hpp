
#pragma once

#include <jni/config.hpp>
#include <jni/java/lang/object.hpp>

#include <jni/ref.hpp>

#include <stdexcept>
#include <string>
#include <iosfwd>

namespace jni {
namespace java {
namespace lang {

class JNI_EXPORT Throwable final : public Object, public std::runtime_error {
public:
  explicit Throwable(local_ref<raw::throwable_ref> &&ref);
  explicit Throwable(global_ref<raw::throwable_ref> &ref);

  Throwable(const Throwable &other);
  Throwable(Throwable &&other);
  virtual ~Throwable();

  Throwable &operator=(const Throwable &other);
  Throwable &operator=(Throwable &&other);

  // Appends the specified exception to the exceptions that were suppressed in
  // order to deliver this exception.
  void addSuppressed(const Throwable &exception);

  // Fills in the execution stack trace.
  Throwable fillInStackTrace();

  // Returns the cause of this throwable or null if the cause is nonexistent or
  // unknown.
  Throwable getCause() const;

  // Creates a localized description of this throwable.
  std::string getLocalizedMessage() const;

  // Returns the detail message string of this throwable.
  std::string getMessage() const;

  // Provides programmatic access to the stack trace information printed by
  // printStackTrace().
  // StackTraceElement[] getStackTrace()

  // Returns an array containing all of the exceptions that were suppressed,
  // typically by the try - with - resources statement, in order to deliver this
  // exception.
  // Throwable[] getSuppressed()

  // Initializes the cause of this throwable to the specified value.
  Throwable initCause(const Throwable &cause);

  // Prints this throwable and its backtrace to the standard error stream.
  void printStackTrace() const;

  // Prints this throwable and its backtrace to the specified print stream.
  void printStackTrace(std::ostream &s) const;

  // Sets the stack trace elements that will be returned by getStackTrace() and
  // printed by printStackTrace() and related methods.
  // void setStackTrace(StackTraceElement[] stackTrace)

  //
  //
  //

  // because we do not want to invoke java methods via the std::exception
  // interface (because that might create new problems), override what()
  // and returned some cached data
  virtual char const* what() const override;

  static void checkEnvironmentAndThrow();

private:
  std::string _serialized_message;
};

} // namespace lang

} // namespace java

} // namespace jni
