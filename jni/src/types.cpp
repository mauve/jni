
#include <jni/raw/types.hpp>

#if JNI_WITH(CHECKED_REFS_ASSERTS)
#include <cassert>
#endif

#include <jni/config.hpp>

namespace jni {
namespace raw {

#if JNI_WITH(CHECKED_REFS)
namespace detail {

//
// _jni_object
//

_jni_object::_jni_object(void *jni_ref, const char *source)
    : _local_refs{0}, _global_refs{0}, _ref{jni_ref}, _source{source} {
  if (!_source) {
    _source = "null-source";
  }
}

_jni_object::~_jni_object() {
#if JNI_WITH(CHECKED_REFS_ASSERTS)
  assert(!_local_refs && !_global_refs);
#endif
}

ref_type _jni_object::type() const { return ref_type::object_type; }

const char *_jni_object::source() const { return _source; }

void _jni_object::add_local_ref() { ++_local_refs; }

void _jni_object::del_local_ref() {
  --_local_refs;
#if JNI_WITH(CHECKED_REFS_ASSERTS)
  assert(_local_refs >= 0);
#endif
}

int _jni_object::local_refs() const { return _local_refs; }

void _jni_object::add_global_ref() { ++_global_refs; }

void _jni_object::del_global_ref() {
  --_global_refs;
#if JNI_WITH(CHECKED_REFS_ASSERTS)
  assert(_global_refs >= 0);
#endif
}

int _jni_object::global_refs() const { return _global_refs; }

void *_jni_object::get_ref() const { return _ref; }

//
// _jni_class
//
_jni_class::_jni_class(void *jni_ref, const char *source)
    : _jni_object{jni_ref, source} {}

ref_type _jni_class::type() const { return ref_type::class_type; }

//
// _jni_throwable
//
_jni_throwable::_jni_throwable(void *jni_ref, const char *source)
    : _jni_object{jni_ref, source} {}

ref_type _jni_throwable::type() const { return ref_type::throwable_type; }

//
// _jni_string
//
_jni_string::_jni_string(void *jni_ref, const char *source)
    : _jni_object{jni_ref, source} {}

ref_type _jni_string::type() const { return ref_type::string_type; }

//
// _jni_array
//
_jni_array::_jni_array(void *jni_ref, const char *source)
    : _jni_object{jni_ref, source} {}

ref_type _jni_array::type() const { return ref_type::array_type; }

} // namespace detail
#endif // JNI_WITH(CHECKED_REFS)

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
