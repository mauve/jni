
#include <jni/detail/throw_if_exception.hpp>
#include <jni/java/lang/throwable.hpp>

namespace jni {

void throw_if_exception() { java::lang::Throwable::checkEnvironmentAndThrow(); }

} // namespace jni
