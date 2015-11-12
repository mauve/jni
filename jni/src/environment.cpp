
#include <jni/environment.hpp>

#include <jni.h>

#include <stdexcept>
#include <string>

#include <boost/system/system_error.hpp>

#include <jni/vm.hpp>
#include "internal/ref_converter.hpp"

namespace jni {

namespace impl {

static int convert_array_release_op(environment::array_release_op op) {
  switch (op) {
  case environment::array_release_op::copy_and_free_buffer:
    return 0;
  case environment::array_release_op::copy_buffer:
    return JNI_COMMIT;
  case environment::array_release_op::free_buffer:
    return JNI_ABORT;
  }

  // default to copy_and_free_buffer
  return 0;
}

class jni_environment : public environment {
public:
  jni_environment(JNIEnv *env) : _env(env) {}
  ~jni_environment() {}

  raw::object_ref new_local_ref(raw::object_ref obj) override {
    if (obj == nullptr)
      return nullptr;
    return internal::from_jni(_env->NewLocalRef(internal::to_jni(obj)),
                              "new_local_ref");
  }

  void delete_local_ref(raw::object_ref obj) override {
    if (obj == nullptr)
      return;
    _env->DeleteLocalRef(internal::to_jni(obj));
  }

  raw::object_ref new_global_ref(raw::object_ref obj) override {
    if (obj == nullptr)
      return nullptr;
    return internal::from_jni(_env->NewGlobalRef(internal::to_jni(obj)),
                              "new_global_ref");
  }

  void delete_global_ref(raw::object_ref obj) override {
    if (obj == nullptr)
      return;
    _env->DeleteGlobalRef(internal::to_jni(obj));
  }

  local_ref<raw::class_ref> find_class(const char *name) override {
    if (name == nullptr)
      return nullptr;
    return make_local_ref(
        internal::from_jni(_env->FindClass(name), "find_class"));
  }

  raw::method_id get_method_id(raw::class_ref cls, const char *name,
                               const char *signature) override {
    if (!name || !signature)
      return nullptr;
    auto res = internal::from_jni(
        _env->GetMethodID(internal::to_jni(cls), name, signature),
        "get_method_id");
    return res;
  }

  raw::method_id get_static_method_id(raw::class_ref cls, const char *name,
                                      const char *signature) override {
    if (!name || !signature)
      return nullptr;
    return internal::from_jni(
        _env->GetStaticMethodID(internal::to_jni(cls), name, signature),
        "get_static_method_id");
  }

  local_ref<raw::class_ref> get_object_class(raw::object_ref obj) override {
    if (!obj)
      return nullptr;
    return make_local_ref(internal::from_jni(
        _env->GetObjectClass(internal::to_jni(obj)), "get_object_class"));
  }

  local_ref<raw::object_ref> new_object(raw::class_ref cls,
                                        raw::method_id constructor,
                                        const raw::value *arguments) {
    if (!cls || !constructor)
      return nullptr;
    return make_local_ref(internal::from_jni(
        _env->NewObjectA(internal::to_jni(cls), internal::to_jni(constructor),
                         reinterpret_cast<const jvalue *>(arguments)),
        "new_object"));
  }

  //
  // array
  //

  local_ref<raw::typed_array_ref<raw::object_ref>>
  new_object_array(std::size_t size) override {
    return nullptr;
  }

  local_ref<raw::object_ref>
  get_object_array_index(raw::typed_array_ref<raw::object_ref> arr,
                         std::size_t index) override {
    return make_local_ref(internal::from_jni(
        _env->GetObjectArrayElement(internal::to_jni(arr),
                                    static_cast<jsize>(index)),
        "get_object_array_index"));
  }

  void set_object_array_index(raw::typed_array_ref<raw::object_ref> arr,
                              std::size_t index, raw::object_ref obj) override {
    _env->SetObjectArrayElement(internal::to_jni(arr),
                                static_cast<jsize>(index),
                                internal::to_jni(obj));
  }

  local_ref<raw::typed_array_ref<bool>> new_array(std::size_t size,
                                                  bool *) override {
    return make_local_ref(
        internal::from_jni(_env->NewBooleanArray(size), "new_array<bool>"));
  }

  local_ref<raw::typed_array_ref<std::uint8_t>>
  new_array(std::size_t size, std::uint8_t *) override {
    return make_local_ref(internal::from_jni(_env->NewByteArray(size),
                                             "new_array<std::uint8_t>"));
  }

  local_ref<raw::typed_array_ref<std::uint16_t>>
  new_array(std::size_t size, std::uint16_t *) override {
    return make_local_ref(internal::from_jni(_env->NewCharArray(size),
                                             "new_array<std::uint16_t>"));
  }

  local_ref<raw::typed_array_ref<std::int16_t>>
  new_array(std::size_t size, std::int16_t *) override {
    return make_local_ref(internal::from_jni(_env->NewShortArray(size),
                                             "new_array<std::int16_t>"));
  }

  local_ref<raw::typed_array_ref<std::int32_t>>
  new_array(std::size_t size, std::int32_t *) override {
    return make_local_ref(
        internal::from_jni(_env->NewIntArray(size), "new_array<std::int32_t>"));
  }

  local_ref<raw::typed_array_ref<std::int64_t>>
  new_array(std::size_t size, std::int64_t *) override {
    return make_local_ref(internal::from_jni(_env->NewLongArray(size),
                                             "new_array<std::int64_t>"));
  }

  local_ref<raw::typed_array_ref<float>> new_array(std::size_t size,
                                                   float *) override {
    return make_local_ref(
        internal::from_jni(_env->NewFloatArray(size), "new_array<float>"));
  }

  local_ref<raw::typed_array_ref<double>> new_array(std::size_t size,
                                                    double *) override {
    return make_local_ref(
        internal::from_jni(_env->NewDoubleArray(size), "new_array<double>"));
  }

  std::size_t get_array_size(raw::array_ref array) override {
    return _env->GetArrayLength(internal::to_jni(array));
  }

  std::pair<bool *, std::size_t>
  get_array_elements(raw::typed_array_ref<bool> arr) override {
    // need this special handling because sizeof(bool) != sizeof(jboolean)
    std::size_t length = get_array_size(arr);
    auto result = new bool[length];
    auto buffer = _env->GetBooleanArrayElements(internal::to_jni(arr), nullptr);
    for (std::size_t i = 0; i < length; ++i) {
      result[i] = !!buffer[i];
    }
    _env->ReleaseBooleanArrayElements(internal::to_jni(arr), buffer, JNI_ABORT);
    return std::make_pair(result, length);
  }

  std::pair<std::uint8_t *, std::size_t>
  get_array_elements(raw::typed_array_ref<std::uint8_t> arr) override {
    return std::make_pair(
        reinterpret_cast<std::uint8_t *>(
            _env->GetByteArrayElements(internal::to_jni(arr), nullptr)),
        get_array_size(arr));
  }

  std::pair<std::uint16_t *, std::size_t>
  get_array_elements(raw::typed_array_ref<std::uint16_t> arr) override {
    return std::make_pair(
        reinterpret_cast<std::uint16_t *>(
            _env->GetCharArrayElements(internal::to_jni(arr), nullptr)),
        get_array_size(arr));
  }

  std::pair<std::int16_t *, std::size_t>
  get_array_elements(raw::typed_array_ref<std::int16_t> arr) override {
    return std::make_pair(
        reinterpret_cast<std::int16_t *>(
            _env->GetShortArrayElements(internal::to_jni(arr), nullptr)),
        get_array_size(arr));
  }

  std::pair<std::int32_t *, std::size_t>
  get_array_elements(raw::typed_array_ref<std::int32_t> arr) override {
    return std::make_pair(
        reinterpret_cast<std::int32_t *>(
            _env->GetIntArrayElements(internal::to_jni(arr), nullptr)),
        get_array_size(arr));
  }

  std::pair<std::int64_t *, std::size_t>
  get_array_elements(raw::typed_array_ref<std::int64_t> arr) override {
    return std::make_pair(
        reinterpret_cast<std::int64_t *>(
            _env->GetLongArrayElements(internal::to_jni(arr), nullptr)),
        get_array_size(arr));
  }

  std::pair<float *, std::size_t>
  get_array_elements(raw::typed_array_ref<float> arr) override {
    return std::make_pair(reinterpret_cast<float *>(_env->GetFloatArrayElements(
                              internal::to_jni(arr), nullptr)),
                          get_array_size(arr));
  }

  std::pair<double *, std::size_t>
  get_array_elements(raw::typed_array_ref<double> arr) override {
    return std::make_pair(
        reinterpret_cast<double *>(
            _env->GetDoubleArrayElements(internal::to_jni(arr), nullptr)),
        get_array_size(arr));
  }

  void release_array_elements(raw::typed_array_ref<bool> arr, bool *elements,
                              array_release_op op) override {
    if (op == array_release_op::free_buffer) {
      delete[] elements;
    } else if (op == array_release_op::copy_buffer ||
               op == array_release_op::copy_and_free_buffer) {
      std::size_t length = get_array_size(arr);
      auto buffer =
          _env->GetBooleanArrayElements(internal::to_jni(arr), nullptr);
      for (std::size_t i = 0; i < length; ++i) {
        buffer[i] = static_cast<jboolean>(elements[i]);
      }
      _env->ReleaseBooleanArrayElements(internal::to_jni(arr), buffer, 0);

      if (op == array_release_op::copy_and_free_buffer)
        delete[] elements;
    }
  }

  void release_array_elements(raw::typed_array_ref<std::uint8_t> arr,
                              std::uint8_t *elements,
                              array_release_op op) override {
    _env->ReleaseByteArrayElements(internal::to_jni(arr),
                                   reinterpret_cast<jbyte *>(elements),
                                   convert_array_release_op(op));
  }

  void release_array_elements(raw::typed_array_ref<std::uint16_t> arr,
                              std::uint16_t *elements,
                              array_release_op op) override {
    _env->ReleaseCharArrayElements(internal::to_jni(arr),
                                   reinterpret_cast<jchar *>(elements),
                                   convert_array_release_op(op));
  }

  void release_array_elements(raw::typed_array_ref<std::int16_t> arr,
                              std::int16_t *elements,
                              array_release_op op) override {
    _env->ReleaseShortArrayElements(internal::to_jni(arr),
                                    reinterpret_cast<jshort *>(elements),
                                    convert_array_release_op(op));
  }

  void release_array_elements(raw::typed_array_ref<std::int32_t> arr,
                              std::int32_t *elements,
                              array_release_op op) override {
    _env->ReleaseIntArrayElements(internal::to_jni(arr),
                                  reinterpret_cast<jint *>(elements),
                                  convert_array_release_op(op));
  }

  void release_array_elements(raw::typed_array_ref<std::int64_t> arr,
                              std::int64_t *elements,
                              array_release_op op) override {
    _env->ReleaseLongArrayElements(internal::to_jni(arr),
                                   reinterpret_cast<jlong *>(elements),
                                   convert_array_release_op(op));
  }

  void release_array_elements(raw::typed_array_ref<float> arr, float *elements,
                              array_release_op op) override {
    _env->ReleaseFloatArrayElements(internal::to_jni(arr),
                                    reinterpret_cast<jfloat *>(elements),
                                    convert_array_release_op(op));
  }

  void release_array_elements(raw::typed_array_ref<double> arr,
                              double *elements, array_release_op op) override {
    _env->ReleaseDoubleArrayElements(internal::to_jni(arr),
                                     reinterpret_cast<jdouble *>(elements),
                                     convert_array_release_op(op));
  }

  //
  // string
  //

  local_ref<raw::string_ref> new_string(const char *characters) override {
    return make_local_ref(
        internal::from_jni(_env->NewStringUTF(characters), "new_string<char>"));
  }

  local_ref<raw::string_ref> new_string(const wchar_t *characters) override {
    return make_local_ref(internal::from_jni(
        _env->NewString(reinterpret_cast<const jchar *>(characters),
                        std::wcslen(characters)),
        "new_string<wchar_t>"));
  }

  std::pair<const char *, std::size_t>
  load_utf8_characters(raw::string_ref string) override {
    if (!string)
      return {nullptr, 0};

    return {_env->GetStringUTFChars(internal::to_jni(string), nullptr),
            _env->GetStringUTFLength(internal::to_jni(string))};
  }

  std::pair<const wchar_t *, std::size_t>
  load_utf16_characters(raw::string_ref string) override {
    if (!string)
      return {nullptr, 0};

    return {reinterpret_cast<const wchar_t *>(
                _env->GetStringChars(internal::to_jni(string), nullptr)),
            _env->GetStringLength(internal::to_jni(string))};
  }

  void release_string_elements(
      raw::string_ref string,
      const std::pair<const char *, std::size_t> &buffer) override {
    if (!string)
      return;

    _env->ReleaseStringUTFChars(internal::to_jni(string), buffer.first);
  }

  void release_string_elements(
      raw::string_ref string,
      const std::pair<const wchar_t *, std::size_t> &buffer) override {
    if (!string)
      return;

    _env->ReleaseStringChars(internal::to_jni(string),
                             reinterpret_cast<const jchar *>(buffer.first));
  }

  //
  // invoke
  //

  bool call_bool_method(raw::object_ref instance, raw::method_id mid,
                        const raw::value *values) override {
    return !!_env->CallBooleanMethodA(internal::to_jni(instance),
                                      internal::to_jni(mid),
                                      reinterpret_cast<const jvalue *>(values));
  }

  std::uint8_t call_byte_method(raw::object_ref instance, raw::method_id mid,
                                const raw::value *values) override {
    return _env->CallByteMethodA(internal::to_jni(instance),
                                 internal::to_jni(mid),
                                 reinterpret_cast<const jvalue *>(values));
  }

  std::uint16_t call_char_method(raw::object_ref instance, raw::method_id mid,
                                 const raw::value *values) override {
    return _env->CallCharMethodA(internal::to_jni(instance),
                                 internal::to_jni(mid),
                                 reinterpret_cast<const jvalue *>(values));
  }

  std::int16_t call_short_method(raw::object_ref instance, raw::method_id mid,
                                 const raw::value *values) override {
    return _env->CallShortMethodA(internal::to_jni(instance),
                                  internal::to_jni(mid),
                                  reinterpret_cast<const jvalue *>(values));
  }

  std::int32_t call_int_method(raw::object_ref instance, raw::method_id mid,
                               const raw::value *values) override {
    return _env->CallIntMethodA(internal::to_jni(instance),
                                internal::to_jni(mid),
                                reinterpret_cast<const jvalue *>(values));
  }

  std::int64_t call_long_method(raw::object_ref instance, raw::method_id mid,
                                const raw::value *values) override {
    return _env->CallLongMethodA(internal::to_jni(instance),
                                 internal::to_jni(mid),
                                 reinterpret_cast<const jvalue *>(values));
  }

  float call_float_method(raw::object_ref instance, raw::method_id mid,
                          const raw::value *values) override {
    return _env->CallFloatMethodA(internal::to_jni(instance),
                                  internal::to_jni(mid),
                                  reinterpret_cast<const jvalue *>(values));
  }

  double call_double_method(raw::object_ref instance, raw::method_id mid,
                            const raw::value *values) override {
    return _env->CallDoubleMethodA(internal::to_jni(instance),
                                   internal::to_jni(mid),
                                   reinterpret_cast<const jvalue *>(values));
  }

  local_ref<raw::object_ref>
  call_object_method(raw::object_ref instance, raw::method_id mid,
                     const raw::value *values) override {
    return make_local_ref(internal::from_jni(
        _env->CallObjectMethodA(internal::to_jni(instance),
                                internal::to_jni(mid),
                                reinterpret_cast<const jvalue *>(values)),
        "call<raw::object_ref>"));
  }

  void call_void_method(raw::object_ref instance, raw::method_id mid,
                        const raw::value *values) override {
    _env->CallVoidMethodA(internal::to_jni(instance), internal::to_jni(mid),
                          reinterpret_cast<const jvalue *>(values));
  }

  //
  // invoke static
  //

  bool call_bool_method(raw::class_ref cls, raw::method_id mid,
                        const raw::value *values) override {
    return !!_env->CallBooleanMethodA(internal::to_jni(cls),
                                      internal::to_jni(mid),
                                      reinterpret_cast<const jvalue *>(values));
  }

  std::uint8_t call_byte_method(raw::class_ref cls, raw::method_id mid,
                                const raw::value *values) override {
    return _env->CallByteMethodA(internal::to_jni(cls), internal::to_jni(mid),
                                 reinterpret_cast<const jvalue *>(values));
  }

  std::uint16_t call_char_method(raw::class_ref cls, raw::method_id mid,
                                 const raw::value *values) override {
    return _env->CallCharMethodA(internal::to_jni(cls), internal::to_jni(mid),
                                 reinterpret_cast<const jvalue *>(values));
  }

  std::int16_t call_short_method(raw::class_ref cls, raw::method_id mid,
                                 const raw::value *values) override {
    return _env->CallShortMethodA(internal::to_jni(cls), internal::to_jni(mid),
                                  reinterpret_cast<const jvalue *>(values));
  }

  std::int32_t call_int_method(raw::class_ref cls, raw::method_id mid,
                               const raw::value *values) override {
    return _env->CallIntMethodA(internal::to_jni(cls), internal::to_jni(mid),
                                reinterpret_cast<const jvalue *>(values));
  }

  std::int64_t call_long_method(raw::class_ref cls, raw::method_id mid,
                                const raw::value *values) override {
    return _env->CallLongMethodA(internal::to_jni(cls), internal::to_jni(mid),
                                 reinterpret_cast<const jvalue *>(values));
  }

  float call_float_method(raw::class_ref cls, raw::method_id mid,
                          const raw::value *values) override {
    return _env->CallFloatMethodA(internal::to_jni(cls), internal::to_jni(mid),
                                  reinterpret_cast<const jvalue *>(values));
  }

  double call_double_method(raw::class_ref cls, raw::method_id mid,
                            const raw::value *values) override {
    return _env->CallDoubleMethodA(internal::to_jni(cls), internal::to_jni(mid),
                                   reinterpret_cast<const jvalue *>(values));
  }

  local_ref<raw::object_ref>
  call_object_method(raw::class_ref cls, raw::method_id mid,
                     const raw::value *values) override {
    return make_local_ref(internal::from_jni(
        _env->CallObjectMethodA(internal::to_jni(cls), internal::to_jni(mid),
                                reinterpret_cast<const jvalue *>(values)),
        "call<raw::object_ref>"));
  }

  void call_void_method(raw::class_ref cls, raw::method_id mid,
                        const raw::value *values) override {
    _env->CallVoidMethodA(internal::to_jni(cls), internal::to_jni(mid),
                          reinterpret_cast<const jvalue *>(values));
  }

  //
  // monitor
  //

  void monitor_enter(raw::object_ref obj) override {
    if (obj == nullptr)
      return;
    _env->MonitorEnter(internal::to_jni(obj));
  }

  void monitor_leave(raw::object_ref obj) override {
    if (obj == nullptr)
      return;
    _env->MonitorExit(internal::to_jni(obj));
  }

  //
  // exceptions
  //

  bool exception_occurred() override { return !!_env->ExceptionCheck(); }

  void clear_exceptions() override { _env->ExceptionClear(); }

  local_ref<raw::throwable_ref> current_exception() override {
    return make_local_ref(
        internal::from_jni(_env->ExceptionOccurred(), "current_exception"));
  }

private:
  JNIEnv *_env;
};

#ifdef JNI_WITH_TESTING
static environment *testing_environment = nullptr;
#endif // JNI_WITH_TESTING

JNIEnv *get_or_create_jni_env(api_version version,
                              const std::string &thread_name) {
  JNIEnv *env = nullptr;

  auto result = vm::current().attach_current_thread(
      thread_name, version, nullptr, reinterpret_cast<void **>(&env));
  if (result) {
    throw boost::system::system_error{result,
                                      "could not attach current thread"};
  }

  return env;
}

} // namespace impl

//
//
//

environment::~environment() {}

environment &environment::current() {
#ifdef JNI_WITH_TESTING
  if (impl::testing_environment) {
    return *impl::testing_environment;
  }
#endif // JNI_WITH_TESTING
  thread_local impl::jni_environment env{
      impl::get_or_create_jni_env(api_version::v1_8, "unnamed thread")};
  return env;
}

#ifdef JNI_WITH_TESTING
void environment::set_testing_environment(environment *env_ptr) {
  impl::testing_environment = env_ptr;
}
#endif // JNI_WITH_TESTING

//
// unsafe
//

namespace unsafe {

void attach(const std::string &thread_name) {
  auto result = vm::current().attach_current_thread(
      thread_name, api_version::v1_8, nullptr, nullptr);
  if (result) {
    throw boost::system::system_error{result,
                                      "could not attach current thread"};
  }
}

void detach() { vm::current().detach_current_thread(); }

} // namespace unsafe

scoped_attach::scoped_attach() : _attached(false) {}
scoped_attach::scoped_attach(const std::string &thread_name)
    : _attached(false) {
  attach(thread_name);
}
scoped_attach::~scoped_attach() { detach(); }

void scoped_attach::attach(const std::string &thread_name) {
  if (_attached)
    return;

  unsafe::attach(thread_name);

  _attached = true;
}

void scoped_attach::detach() {
  if (!_attached)
    return;

  unsafe::detach();

  _attached = false;
}

} // namespace jni
