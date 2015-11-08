
#include "ref_converter.hpp"

#include <jni/raw/types.hpp>
#include <jni/vm.hpp>

#if JNI_WITH(CHECKED_REFS_ASSERTS)
#include <cassert>
#endif
#include <type_traits>
#include <unordered_map>

#include <jni.h>

namespace jni {
namespace internal {

#if JNI_WITH(CHECKED_REFS)
struct ref_registry {
  static JNIEnv *get_jni_env() {
    JNIEnv *env = nullptr;
    vm::current().get_current_env(api_version::v1_1,
                                  reinterpret_cast<void **>(&env));
    assert(env);
    return env;
  }

#if JNI_WITH(CHECKED_REFS_ASSERTS)
  ~ref_registry() { assert(_registry.empty() && "leaked refs"); }
#endif

  template <typename ImplType>
  ImplType *from_jni(const char *source, jobject jobj) {
    static_assert(
        std::is_convertible<ImplType, raw::detail::_jni_object>::value,
        "RefType must be convertible to raw::detail::_jni_object");

    if (jobj == nullptr)
      return nullptr;

    auto iterator = _registry.find(jobj);
    if (iterator == _registry.end()) {
      // we cannot use environment::current() here because our
      // JNIEnv wrapper uses the checked refs but we need to inspect
      // the underlying raw ref
      auto checked = new ImplType{jobj, source};

      auto ref_type = get_jni_env()->GetObjectRefType(jobj);
      switch (ref_type) {
      case JNIInvalidRefType:
        delete checked;
        return nullptr;
      case JNILocalRefType:
        checked->add_local_ref();
        break;
      case JNIGlobalRefType:
        checked->add_global_ref();
        break;
      }

      _registry.insert(std::make_pair(jobj, checked));
      return checked;
    }

    auto result = dynamic_cast<ImplType *>(iterator->second);
    assert(result);
    return result;
  }

  jobject to_jni(raw::object_ref ref) {
    if (ref == nullptr)
      return nullptr;

    return reinterpret_cast<jobject>(ref->get_ref());
  }

  static ref_registry &get() {
    static ref_registry reg;
    return reg;
  }

private:
  std::unordered_map<jobject, raw::object_ref> _registry;
};

raw::object_ref from_jni(jobject jobj, raw::ref_type type, const char *source) {
  return ref_registry::get().from_jni<raw::detail::_jni_object>(source, jobj);
}

raw::object_ref from_jni(jobject jref, const char *source) {
  return ref_registry::get().from_jni<raw::detail::_jni_object>(source, jref);
}

jobject to_jni(raw::object_ref ref) {
  return reinterpret_cast<jobject>(ref->get_ref());
}

raw::class_ref from_jni(jclass jref, const char *source) {
  return ref_registry::get().from_jni<raw::detail::_jni_class>(source, jref);
}

jclass to_jni(raw::class_ref ref) {
  return reinterpret_cast<jclass>(ref->get_ref());
}

raw::throwable_ref from_jni(jthrowable jref, const char *source) {
  return ref_registry::get().from_jni<raw::detail::_jni_throwable>(source,
                                                                   jref);
}

jthrowable to_jni(raw::throwable_ref ref) {
  return reinterpret_cast<jthrowable>(ref->get_ref());
}

raw::string_ref from_jni(jstring jref, const char *source) {
  return ref_registry::get().from_jni<raw::detail::_jni_string>(source, jref);
}

jstring to_jni(raw::string_ref ref) {
  return reinterpret_cast<jstring>(ref->get_ref());
}

raw::array_ref from_jni(jarray jref, const char *source) {
  return ref_registry::get().from_jni<raw::detail::_jni_array>(source, jref);
}

jarray to_jni(raw::array_ref ref, const char *source) {
  return reinterpret_cast<jarray>(ref->get_ref());
}

raw::method_id from_jni(jmethodID mid, const char *source) {
  return reinterpret_cast<raw::method_id>(mid);
}

raw::field_id from_jni(jfieldID fid, const char *source) {
  return reinterpret_cast<raw::field_id>(fid);
}

jmethodID to_jni(raw::method_id mid) {
  return reinterpret_cast<jmethodID>(mid);
}

jfieldID to_jni(raw::field_id fid) { return reinterpret_cast<jfieldID>(fid); }

raw::typed_array_ref<raw::object_ref> from_jni(jobjectArray jref,
                                               const char *source) {
  return ref_registry::get()
      .from_jni<raw::detail::_jni_typed_array<raw::object_ref>>(source, jref);
}

jobjectArray to_jni(raw::typed_array_ref<raw::object_ref> ref) {
  return reinterpret_cast<jobjectArray>(ref->get_ref());
}

raw::typed_array_ref<bool> from_jni(jbooleanArray jref, const char *source) {
  return ref_registry::get().from_jni<raw::detail::_jni_typed_array<bool>>(
      source, jref);
}

jbooleanArray to_jni(raw::typed_array_ref<bool> ref) {
  return reinterpret_cast<jbooleanArray>(ref->get_ref());
}

raw::typed_array_ref<std::uint8_t> from_jni(jbyteArray jref,
                                            const char *source) {
  return ref_registry::get()
      .from_jni<raw::detail::_jni_typed_array<std::uint8_t>>(source, jref);
}

jbyteArray to_jni(raw::typed_array_ref<std::uint8_t> ref) {
  return reinterpret_cast<jbyteArray>(ref->get_ref());
}

raw::typed_array_ref<std::uint16_t> from_jni(jcharArray jref,
                                             const char *source) {
  return ref_registry::get()
      .from_jni<raw::detail::_jni_typed_array<std::uint16_t>>(source, jref);
}

jcharArray to_jni(raw::typed_array_ref<std::uint16_t> ref) {
  return reinterpret_cast<jcharArray>(ref->get_ref());
}

raw::typed_array_ref<std::int16_t> from_jni(jshortArray jref,
                                            const char *source) {
  return ref_registry::get()
      .from_jni<raw::detail::_jni_typed_array<std::int16_t>>(source, jref);
}

jshortArray to_jni(raw::typed_array_ref<std::int16_t> ref) {
  return reinterpret_cast<jshortArray>(ref->get_ref());
}

raw::typed_array_ref<std::int32_t> from_jni(jintArray jref,
                                            const char *source) {
  return ref_registry::get()
      .from_jni<raw::detail::_jni_typed_array<std::int32_t>>(source, jref);
}

jintArray to_jni(raw::typed_array_ref<std::int32_t> ref) {
  return reinterpret_cast<jintArray>(ref->get_ref());
}

raw::typed_array_ref<std::int64_t> from_jni(jlongArray jref,
                                            const char *source) {
  return ref_registry::get()
      .from_jni<raw::detail::_jni_typed_array<std::int64_t>>(source, jref);
}

jlongArray to_jni(raw::typed_array_ref<std::int64_t> ref) {
  return reinterpret_cast<jlongArray>(ref->get_ref());
}

raw::typed_array_ref<float> from_jni(jfloatArray jref, const char *source) {
  return ref_registry::get().from_jni<raw::detail::_jni_typed_array<float>>(
      source, jref);
}

jfloatArray to_jni(raw::typed_array_ref<float> ref) {
  return reinterpret_cast<jfloatArray>(ref->get_ref());
}

raw::typed_array_ref<double> from_jni(jdoubleArray jref, const char *source) {
  return ref_registry::get().from_jni<raw::detail::_jni_typed_array<double>>(
      source, jref);
}

jdoubleArray to_jni(raw::typed_array_ref<double> ref) {
  return reinterpret_cast<jdoubleArray>(ref->get_ref());
}

#endif // JNI_WITH(CHECKED_REFS)

} // namespace internal
} // namespace jni
