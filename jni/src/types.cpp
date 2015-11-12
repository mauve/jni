
#include <jni/raw/types.hpp>

#include <jni/config.hpp>

namespace jni {
namespace raw {

#define DEFINE_SIGNATURE(type, str) template<> JNI_EXPORT const char *get_signature<type>::get() { return str; }

DEFINE_SIGNATURE(bool, "Z");
DEFINE_SIGNATURE(std::uint8_t, "B");
DEFINE_SIGNATURE(std::uint16_t, "C");
DEFINE_SIGNATURE(std::int16_t, "S");
DEFINE_SIGNATURE(std::int32_t, "I");
DEFINE_SIGNATURE(std::int64_t, "J");
DEFINE_SIGNATURE(float, "F");
DEFINE_SIGNATURE(double, "D");
DEFINE_SIGNATURE(void, "V");
DEFINE_SIGNATURE(object_ref, "Ljava/lang/Object;");
DEFINE_SIGNATURE(string_ref, "Ljava/lang/String;");
DEFINE_SIGNATURE(class_ref, "Ljava/lang/Class;");
DEFINE_SIGNATURE(throwable_ref, "Ljava/lang/Throwable;");

DEFINE_SIGNATURE(typed_array_ref<bool>, "[Z");
DEFINE_SIGNATURE(typed_array_ref<std::uint8_t>, "[B");
DEFINE_SIGNATURE(typed_array_ref<std::uint16_t>, "[C");
DEFINE_SIGNATURE(typed_array_ref<std::int16_t>, "[S");
DEFINE_SIGNATURE(typed_array_ref<std::int32_t>, "[I");
DEFINE_SIGNATURE(typed_array_ref<std::int64_t>, "[J");
DEFINE_SIGNATURE(typed_array_ref<float>, "[F");
DEFINE_SIGNATURE(typed_array_ref<double>, "[D");
DEFINE_SIGNATURE(typed_array_ref<object_ref>, "[Ljava/lang/Object;");
DEFINE_SIGNATURE(typed_array_ref<string_ref>, "[Ljava/lang/String;");
DEFINE_SIGNATURE(typed_array_ref<class_ref>, "[Ljava/lang/Class;");
DEFINE_SIGNATURE(typed_array_ref<throwable_ref>, "[Ljava/lang/Throwable;");

#undef DEFINE_SIGNATURE

} // namespace raw
} // namespace jni
