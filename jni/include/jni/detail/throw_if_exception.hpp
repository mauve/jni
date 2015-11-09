
#pragma once

#include <jni/config.hpp>

namespace jni {

// throws a java::lang::Throwable or more derived exception
// if there is currently an exception pending on the environment
JNI_EXPORT void throw_if_exception();

} // namespace jni
