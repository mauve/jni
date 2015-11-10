
#include <jni/raw/object_ref_array.hpp>

#include <jni/environment.hpp>

namespace jni {
namespace raw {

object_ref_array::object_ref_array(local_ref<typed_array_ref<object_ref>> &&ref)
    : _size{environment::current().get_array_size(ref.raw())},
      _ref(std::move(ref)) {}

object_ref_array::object_ref_array(
    global_ref<typed_array_ref<object_ref>> &&ref)
    : _size{environment::current().get_array_size(ref.raw())},
      _ref(std::move(ref)) {}

object_ref_array::object_ref_array(const object_ref_array &copy) = default;

object_ref_array::object_ref_array(object_ref_array &&other) = default;

object_ref_array::~object_ref_array() = default;

object_ref_array &object_ref_array::
operator=(const object_ref_array &) = default;

object_ref_array &object_ref_array::operator=(object_ref_array &&) = default;

std::size_t object_ref_array::size() const { return _size; }

local_ref<object_ref> object_ref_array::get(std::size_t index) const {
  if (index >= _size)
    throw std::out_of_range("object_ref_array out of range get() access");
  return environment::current().get_object_array_index(_ref.raw(), index);
}

void object_ref_array::set(std::size_t index, object_ref ref) {
  if (index >= _size)
    throw std::out_of_range("object_ref_array out of range set() access");
  environment::current().set_object_array_index(_ref.raw(), index, ref);
}

typed_array_ref<object_ref> object_ref_array::ref() const {
  return _ref.raw();
}

} // namespace raw
} // namespace jni
