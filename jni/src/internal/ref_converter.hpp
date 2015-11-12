
#pragma once

#include <jni/raw/types.hpp>

#include <jni.h>

namespace jni {
namespace internal {

inline raw::object_ref from_jni(jobject jobj, raw::ref_type type, const char *source) {
  return reinterpret_cast<raw::object_ref>(jobj);
}

inline raw::object_ref from_jni(jobject jobj, const char *source) {
  return reinterpret_cast<raw::object_ref>(jobj);
}

inline raw::class_ref from_jni(jclass jcls, const char *source) {
  return reinterpret_cast<raw::class_ref>(jcls);
}

inline raw::throwable_ref from_jni(jthrowable jthr, const char *source) {
  return reinterpret_cast<raw::throwable_ref>(jthr);
}

inline raw::array_ref from_jni(jarray jarr, const char *source) {
  return reinterpret_cast<raw::array_ref>(jarr);
}

inline raw::typed_array_ref<raw::object_ref> from_jni(jobjectArray ref, const char *source) {
  return reinterpret_cast<raw::typed_array_ref<raw::object_ref>>(ref);
}

inline raw::typed_array_ref<bool> from_jni(jbooleanArray ref, const char *source) {
  return reinterpret_cast<raw::typed_array_ref<bool>>(ref);
}

inline raw::typed_array_ref<std::uint8_t> from_jni(jbyteArray ref, const char *source) {
  return reinterpret_cast<raw::typed_array_ref<std::uint8_t>>(ref);
}

inline raw::typed_array_ref<std::uint16_t> from_jni(jcharArray ref, const char *source) {
  return reinterpret_cast<raw::typed_array_ref<std::uint16_t>>(ref);
}

inline raw::typed_array_ref<std::int16_t> from_jni(jshortArray ref, const char *source) {
  return reinterpret_cast<raw::typed_array_ref<std::int16_t>>(ref);
}

inline raw::typed_array_ref<std::int32_t> from_jni(jintArray ref, const char *source) {
  return reinterpret_cast<raw::typed_array_ref<std::int32_t>>(ref);
}

inline raw::typed_array_ref<std::int64_t> from_jni(jlongArray ref, const char *source) {
  return reinterpret_cast<raw::typed_array_ref<std::int64_t>>(ref);
}

inline raw::typed_array_ref<float> from_jni(jfloatArray ref, const char *source) {
  return reinterpret_cast<raw::typed_array_ref<float>>(ref);
}

inline raw::typed_array_ref<double> from_jni(jdoubleArray ref, const char *source) {
  return reinterpret_cast<raw::typed_array_ref<double>>(ref);
}

inline raw::string_ref from_jni(jstring jstr, const char *source) {
  return reinterpret_cast<raw::string_ref>(jstr);
}

inline raw::method_id from_jni(jmethodID mid, const char *source) {
  return reinterpret_cast<raw::method_id>(mid);
}

inline raw::field_id from_jni(jfieldID fid, const char *source) {
  return reinterpret_cast<raw::field_id>(fid);
}

inline jobject to_jni(raw::object_ref ref) {
  return reinterpret_cast<jobject>(ref);
}

inline jclass to_jni(raw::class_ref ref) {
  return reinterpret_cast<jclass>(ref);
}

inline jthrowable to_jni(raw::throwable_ref ref) {
  return reinterpret_cast<jthrowable>(ref);
}

inline jarray to_jni(raw::array_ref ref) {
  return reinterpret_cast<jarray>(ref);
}

inline jobjectArray to_jni(raw::typed_array_ref<raw::object_ref> ref) {
  return reinterpret_cast<jobjectArray>(ref);
}

inline jbooleanArray to_jni(raw::typed_array_ref<bool> ref) {
  return reinterpret_cast<jbooleanArray>(ref);
}

inline jbyteArray to_jni(raw::typed_array_ref<std::uint8_t> ref) {
  return reinterpret_cast<jbyteArray>(ref);
}

inline jcharArray to_jni(raw::typed_array_ref<std::uint16_t> ref) {
  return reinterpret_cast<jcharArray>(ref);
}

inline jshortArray to_jni(raw::typed_array_ref<std::int16_t> ref) {
  return reinterpret_cast<jshortArray>(ref);
}

inline jintArray to_jni(raw::typed_array_ref<std::int32_t> ref) {
  return reinterpret_cast<jintArray>(ref);
}

inline jlongArray to_jni(raw::typed_array_ref<std::int64_t> ref) {
  return reinterpret_cast<jlongArray>(ref);
}

inline jfloatArray to_jni(raw::typed_array_ref<float> ref) {
  return reinterpret_cast<jfloatArray>(ref);
}

inline jdoubleArray to_jni(raw::typed_array_ref<double> ref) {
  return reinterpret_cast<jdoubleArray>(ref);
}

inline jstring to_jni(raw::string_ref ref) {
  return reinterpret_cast<jstring>(ref);
}

inline jmethodID to_jni(raw::method_id mid) {
  return reinterpret_cast<jmethodID>(mid);
}

inline jfieldID to_jni(raw::field_id fid) {
  return reinterpret_cast<jfieldID>(fid);
}

} // namespace internal
} // namespace jni
