
#pragma once

#include <jni/api_version.hpp>

namespace jni {

class global_initialization;

namespace init {

api_version on_load(void *vm);
void on_unload(void *vm);

} // namespace init

} // namespace jni

#if !defined(JNI_USER_LOAD_HOOK) && !defined(JNI_BUILDING)
# define INIT_INCLUDING
# include <jni/impl/init.ipp>
# undef INIT_INCLUDING
#endif
