
#pragma once

#include <jni/config.hpp>
#include <jni/ref.hpp>
#include <jni/raw/types.hpp>

#include <type_traits>

namespace jni {
namespace raw {

class JNI_EXPORT object_ref_array {
public:
  object_ref_array(local_ref<typed_array_ref<object_ref>> &&ref);
  object_ref_array(global_ref<typed_array_ref<object_ref>> &&ref);
  object_ref_array(const object_ref_array &copy);
  object_ref_array(object_ref_array &&other);
  ~object_ref_array();

  object_ref_array &operator=(const object_ref_array &other);
  object_ref_array &operator=(object_ref_array &&other);

  std::size_t size() const;

  local_ref<object_ref> get(std::size_t index) const;
  void set(std::size_t index, object_ref ref);

  friend typed_array_ref<object_ref>
      JNI_EXPORT extract_reference(const object_ref_array &);

private:
  std::size_t _size;
  global_ref<typed_array_ref<object_ref>> _ref;
};

} // namespace raw
} // namespace jni
