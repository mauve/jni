
#ifndef INIT_INCLUDING
#error "init.ipp cannot be included directly"
#endif

#include <cstdint>

// TODO: xplat
#define JNI_EXPORT __declspec(dllexport)
#define JNI_CALLCONV __stdcall

extern "C" {
inline JNI_EXPORT std::int32_t JNI_CALLCONV JNI_OnLoad(void *vm,
                                                       void *reserved) {
  return static_cast<std::int32_t>(jni::init::on_load(vm));
}

inline JNI_EXPORT void JNI_CALLCONV JNI_OnUnLoad(void *vm, void *reserved) {
  jni::init::on_unload(vm);
}
} // extern "C"

#undef JNI_EXPORT
#undef JNI_CALLCONV
