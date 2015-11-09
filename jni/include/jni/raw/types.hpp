
#pragma once

#include <jni/config.hpp>

#include <cstdint>

namespace jni {

// forward decl
template <typename T> class local_ref;
template <typename T> class global_ref;

namespace raw {

enum ref_type {
  object_type,
  class_type,
  string_type,
  throwable_type,
  array_type
};

namespace detail {

struct _jni_object {
#if JNI_WITH(CHECKED_REFS)
  _jni_object() = delete;
  _jni_object(const _jni_object &) = delete;
  _jni_object(_jni_object &&) = delete;

  _jni_object &operator= (const _jni_object &) = delete;
  _jni_object &operator= (_jni_object &&) = delete;

  _jni_object(void *jni_ref, const char *source);
  virtual ~_jni_object();

  virtual ref_type type() const;
  virtual const char *source() const;

  virtual void add_local_ref();
  virtual void del_local_ref();
  virtual int local_refs() const;

  virtual void add_global_ref();
  virtual void del_global_ref();
  virtual int global_refs() const;

  virtual void *get_ref() const;

private:
  int _local_refs;
  int _global_refs;
  void *_ref;
  const char *_source;
#endif
};

struct _jni_class : public _jni_object {
#if JNI_WITH(CHECKED_REFS)
  explicit _jni_class(void *jni_ref, const char *source);

  ref_type type() const override;
#endif
};

struct _jni_throwable : public _jni_object {
#if JNI_WITH(CHECKED_REFS)
  explicit _jni_throwable(void *jni_ref, const char *source);

  ref_type type() const override;
#endif
};

struct _jni_string : public _jni_object {
#if JNI_WITH(CHECKED_REFS)
  explicit _jni_string(void *jni_ref, const char *source);

  ref_type type() const override;
#endif
};

struct _jni_array : public _jni_object {
#if JNI_WITH(CHECKED_REFS)
  explicit _jni_array(void *jni_ref, const char *source);

  ref_type type() const override;
#endif
};

template <typename T> class _jni_typed_array : public _jni_array {
  using _jni_array::_jni_array;
};

class _jni_method_id {};
class _jni_field_id {};

} // namespace detail

using object_ref = detail::_jni_object *;
using class_ref = detail::_jni_class *;
using throwable_ref = detail::_jni_throwable *;
using string_ref = detail::_jni_string *;
using array_ref = detail::_jni_array *;

template <typename T> using typed_array_ref = detail::_jni_typed_array<T> *;

using method_id = detail::_jni_method_id *;
using field_id = detail::_jni_field_id *;

// returns the JNI signature for the given type, e.g.
//   get_signature<bool>::get() returns "Z"
template <typename T> struct get_signature {
  static const char *get();
};

extern template const char * get_signature<bool>::get();
extern template const char * get_signature<std::uint8_t>::get();
extern template const char * get_signature<std::uint16_t>::get();
extern template const char * get_signature<std::int16_t>::get();
extern template const char * get_signature<std::int32_t>::get();
extern template const char * get_signature<std::int64_t>::get();
extern template const char * get_signature<float>::get();
extern template const char * get_signature<double>::get();
extern template const char * get_signature<void>::get();
extern template const char * get_signature<object_ref>::get();
extern template const char * get_signature<string_ref>::get();
extern template const char * get_signature<class_ref>::get();
extern template const char * get_signature<throwable_ref>::get();

extern template const char * get_signature<typed_array_ref<bool>>::get();
extern template const char * get_signature<typed_array_ref<std::uint8_t>>::get();
extern template const char * get_signature<typed_array_ref<std::uint16_t>>::get();
extern template const char * get_signature<typed_array_ref<std::int16_t>>::get();
extern template const char * get_signature<typed_array_ref<std::int32_t>>::get();
extern template const char * get_signature<typed_array_ref<std::int64_t>>::get();
extern template const char * get_signature<typed_array_ref<float>>::get();
extern template const char * get_signature<typed_array_ref<double>>::get();
extern template const char * get_signature<typed_array_ref<object_ref>>::get();
extern template const char * get_signature<typed_array_ref<string_ref>>::get();
extern template const char * get_signature<typed_array_ref<class_ref>>::get();
extern template const char * get_signature<typed_array_ref<throwable_ref>>::get();

} // namespace raw
} // namespace jni
