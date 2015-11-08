
#pragma once

#include <jni/raw/types.hpp>
#include <jni/ref.hpp>

#include <type_traits>

namespace jni {

namespace raw {

template <typename T> class ref_holder;

template <typename T> T get_ref(const ref_holder<T> &);
template <typename T> T get_ref(const ref_holder<T> *);

template <typename T> class JNI_EXPORT ref_holder {
  static_assert(std::is_convertible<T, object_ref>::value,
                "T must be convertible to jni::raw::object_ref");

protected:
  ref_holder(std::nullptr_t);
  ref_holder(global_ref<T> &&ref);
  ref_holder(local_ref<T> &&ref);
  ref_holder(const ref_holder &other) = default;
  ref_holder(ref_holder &&other) = default;
  ~ref_holder() = default;

  ref_holder &operator=(const ref_holder &other) = default;
  ref_holder &operator=(ref_holder &&other) = default;

  T get_ref() const;

private:
  template <typename T> friend T get_ref(const ref_holder<T> &);
  template <typename T> friend T get_ref(const ref_holder<T> *);

  global_ref<T> _ref;
};

} // namespace raw
} // namespace jni

#define REF_HOLDER_INCLUDING
#include <jni/impl/raw/ref_holder.ipp>
#undef REF_HOLDER_INCLUDING
