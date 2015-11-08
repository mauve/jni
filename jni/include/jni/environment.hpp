#pragma once

#include <string>

#include <jni/config.hpp>
#include <jni/api_version.hpp>
#include <jni/raw/types.hpp>
#include <jni/raw/value.hpp>
#include <jni/ref.hpp>

namespace jni {

class JNI_EXPORT environment {
public:
  virtual ~environment() = 0;

  // returns nullptr if obj == nullptr
  virtual raw::object_ref new_local_ref(raw::object_ref obj) = 0;
  // does nothing if obj == nullptr
  virtual void delete_local_ref(raw::object_ref obj) = 0;

  // returns nullptr if obj == nullptr
  virtual raw::object_ref new_global_ref(raw::object_ref obj) = 0;
  // does nothing if obj == nullptr
  virtual void delete_global_ref(raw::object_ref obj) = 0;

  virtual raw::class_ref find_class(const char *name) = 0;

  virtual raw::method_id get_method_id(raw::class_ref cls, const char *name,
                                       const char *signature) = 0;

  virtual raw::method_id get_static_method_id(raw::class_ref cls,
                                              const char *name,
                                              const char *signature) = 0;

  virtual raw::class_ref get_object_class(raw::object_ref obj) = 0;

  virtual raw::object_ref new_object(raw::class_ref cls,
                                     raw::method_id constructor,
                                     const raw::value *arguments) = 0;

  //
  // array methods
  //
  virtual raw::typed_array_ref<raw::object_ref>
  new_object_array(std::size_t size) = 0;
  virtual raw::object_ref
  get_object_array_index(raw::typed_array_ref<raw::object_ref> arr,
                         std::size_t index) = 0;
  virtual void set_object_array_index(raw::typed_array_ref<raw::object_ref> arr,
                                      std::size_t index,
                                      raw::object_ref obj) = 0;

  // second argument only used for overloading
  virtual raw::typed_array_ref<bool> new_array(std::size_t size, bool *) = 0;
  virtual raw::typed_array_ref<std::uint8_t> new_array(std::size_t size,
                                                       std::uint8_t *) = 0;
  virtual raw::typed_array_ref<std::uint16_t> new_array(std::size_t size,
                                                        std::uint16_t *) = 0;
  virtual raw::typed_array_ref<std::int16_t> new_array(std::size_t size,
                                                       std::int16_t *) = 0;
  virtual raw::typed_array_ref<std::int32_t> new_array(std::size_t size,
                                                       std::int32_t *) = 0;
  virtual raw::typed_array_ref<std::int64_t> new_array(std::size_t size,
                                                       std::int64_t *) = 0;
  virtual raw::typed_array_ref<float> new_array(std::size_t size, float *) = 0;
  virtual raw::typed_array_ref<double> new_array(std::size_t size,
                                                 double *) = 0;

  virtual std::size_t get_array_size(raw::array_ref array) = 0;

  virtual std::pair<bool *, std::size_t>
  get_array_elements(raw::typed_array_ref<bool> arr) = 0;
  virtual std::pair<std::uint8_t *, std::size_t>
  get_array_elements(raw::typed_array_ref<std::uint8_t> arr) = 0;
  virtual std::pair<std::uint16_t *, std::size_t>
  get_array_elements(raw::typed_array_ref<std::uint16_t> arr) = 0;
  virtual std::pair<std::int16_t *, std::size_t>
  get_array_elements(raw::typed_array_ref<std::int16_t> arr) = 0;
  virtual std::pair<std::int32_t *, std::size_t>
  get_array_elements(raw::typed_array_ref<std::int32_t> arr) = 0;
  virtual std::pair<std::int64_t *, std::size_t>
  get_array_elements(raw::typed_array_ref<std::int64_t> arr) = 0;
  virtual std::pair<float *, std::size_t>
  get_array_elements(raw::typed_array_ref<float> arr) = 0;
  virtual std::pair<double *, std::size_t>
  get_array_elements(raw::typed_array_ref<double> arr) = 0;

  enum class array_release_op {
    copy_and_free_buffer,
    copy_buffer,
    free_buffer
  };

  virtual void release_array_elements(raw::typed_array_ref<bool> arr, bool *,
                                      array_release_op op) = 0;
  virtual void release_array_elements(raw::typed_array_ref<std::uint8_t> arr,
                                      std::uint8_t *, array_release_op op) = 0;
  virtual void release_array_elements(raw::typed_array_ref<std::uint16_t> arr,
                                      std::uint16_t *, array_release_op op) = 0;
  virtual void release_array_elements(raw::typed_array_ref<std::int16_t> arr,
                                      std::int16_t *, array_release_op op) = 0;
  virtual void release_array_elements(raw::typed_array_ref<std::int32_t> arr,
                                      std::int32_t *, array_release_op op) = 0;
  virtual void release_array_elements(raw::typed_array_ref<std::int64_t> arr,
                                      std::int64_t *, array_release_op op) = 0;
  virtual void release_array_elements(raw::typed_array_ref<float> arr, float *,
                                      array_release_op op) = 0;
  virtual void release_array_elements(raw::typed_array_ref<double> arr,
                                      double *, array_release_op op) = 0;

  //
  // string
  //

  virtual raw::string_ref new_string(const char *characters) = 0;
  virtual raw::string_ref new_string(const wchar_t *characters) = 0;

  virtual std::pair<const char *, std::size_t>
  load_utf8_characters(raw::string_ref string) = 0;
  virtual std::pair<const wchar_t *, std::size_t>
  load_utf16_characters(raw::string_ref string) = 0;

  virtual void release_string_elements(
      raw::string_ref string,
      const std::pair<const char *, std::size_t> &buffer) = 0;
  virtual void release_string_elements(
      raw::string_ref string,
      const std::pair<const wchar_t *, std::size_t> &buffer) = 0;

  //
  // invoking
  //

  virtual bool call_bool_method(raw::object_ref instance, raw::method_id mid,
                                const raw::value *values) = 0;
  virtual std::uint8_t call_byte_method(raw::object_ref instance,
                                        raw::method_id mid,
                                        const raw::value *values) = 0;
  virtual std::uint16_t call_char_method(raw::object_ref instance,
                                         raw::method_id mid,
                                         const raw::value *values) = 0;
  virtual std::int16_t call_short_method(raw::object_ref instance,
                                         raw::method_id mid,
                                         const raw::value *values) = 0;
  virtual std::int32_t call_int_method(raw::object_ref instance,
                                       raw::method_id mid,
                                       const raw::value *values) = 0;
  virtual std::int64_t call_long_method(raw::object_ref instance,
                                        raw::method_id mid,
                                        const raw::value *values) = 0;
  virtual float call_float_method(raw::object_ref instance, raw::method_id mid,
                                  const raw::value *values) = 0;
  virtual double call_double_method(raw::object_ref instance,
                                    raw::method_id mid,
                                    const raw::value *values) = 0;
  virtual raw::object_ref call_object_method(raw::object_ref instance,
                                             raw::method_id mid,
                                             const raw::value *values) = 0;
  virtual void call_void_method(raw::object_ref instance, raw::method_id mid,
                                const raw::value *values) = 0;

  //
  // invoke static methods
  //

  virtual bool call_bool_method(raw::class_ref cls, raw::method_id mid,
                                const raw::value *values) = 0;
  virtual std::uint8_t call_byte_method(raw::class_ref cls, raw::method_id mid,
                                        const raw::value *values) = 0;
  virtual std::uint16_t call_char_method(raw::class_ref cls, raw::method_id mid,
                                         const raw::value *values) = 0;
  virtual std::int16_t call_short_method(raw::class_ref cls, raw::method_id mid,
                                         const raw::value *values) = 0;
  virtual std::int32_t call_int_method(raw::class_ref cls, raw::method_id mid,
                                       const raw::value *values) = 0;
  virtual std::int64_t call_long_method(raw::class_ref cls, raw::method_id mid,
                                        const raw::value *values) = 0;
  virtual float call_float_method(raw::class_ref cls, raw::method_id mid,
                                  const raw::value *values) = 0;
  virtual double call_double_method(raw::class_ref cls, raw::method_id mid,
                                    const raw::value *values) = 0;
  virtual raw::object_ref call_object_method(raw::class_ref cls,
                                             raw::method_id mid,
                                             const raw::value *values) = 0;
  virtual void call_void_method(raw::class_ref cls, raw::method_id mid,
                                const raw::value *values) = 0;

  //
  // monitor
  //

  virtual void monitor_enter(raw::object_ref obj) = 0;

  virtual void monitor_leave(raw::object_ref obj) = 0;

  // returns the current thread-local environment, or attaches
  // the thread if no environment has been created for this thread
  // yet, you must never transfer an environment from one thread
  // to another
  static environment &current();

#ifdef JNI_WITH_TESTING
  static void set_testing_environment(environment *env);
#endif // JNI_WITH_TESTING
};

namespace unsafe {

//
// do not use these functions, prefer scoped_attach instead
//

// attaches the current thread to the vm, if it has already been
// attached this function is a noop. you do only need to to use
// this function if  you want to override the default
// api_version (1.4) and thread_name (unnamed thread)
void JNI_EXPORT attach(const std::string &thread_name);

// detaches the current thread from the vm, everybody waiting
// for the Java thread associated with this native thread to die
// will be notified and all monitors will be closed
void JNI_EXPORT detach();

} // namespace unsafe

class JNI_EXPORT scoped_attach {
private:
  scoped_attach();
  scoped_attach(const std::string &thread_name);
  ~scoped_attach();

  void attach(const std::string &thread_name);
  void detach();

  scoped_attach(const scoped_attach &) = delete;
  scoped_attach(scoped_attach &&) = delete;

  scoped_attach &operator=(const scoped_attach &) = delete;
  scoped_attach &operator=(scoped_attach &&) = delete;

private:
  bool _attached;
};

} // namespace jni