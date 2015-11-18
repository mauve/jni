
#include <jni/detail/throw_if_exception.hpp>
#include <jni/java/lang/throwable.hpp>
#include <jni/environment.hpp>

namespace jni {

void throw_if_exception() {
  using java::lang::Throwable;

  if (!environment::current().exception_occurred())
    return;

  auto current_exception = environment::current().current_exception();
  environment::current().clear_exceptions();
  if (!current_exception)
    return;

  Throwable throwable{std::move(current_exception)};
#ifdef _DEBUG
  throwable.printStackTrace();
#endif
  throw throwable;
}

} // namespace jni
