
#pragma once

#include <jni/raw/types.hpp>

#include <type_traits>

namespace jni {

class environment;

template <typename T> class local_ref;
template <typename T> class global_ref;

template <typename T> class local_ref {

  static_assert(std::is_convertible<T, raw::object_ref>::value,
                "T must be convertible to jni::raw::object_ref");

public:
  local_ref();
  local_ref(std::nullptr_t);
  explicit local_ref(T ref);
  local_ref(const local_ref &other);
  local_ref(local_ref &&other);
  local_ref(global_ref<T> &&other);
  ~local_ref();

  void release();

  local_ref &operator=(const local_ref &other);
  local_ref &operator=(local_ref &&other);
  local_ref &operator=(const global_ref<T> &other);
  local_ref &operator=(global_ref<T> &&other);

  void assign(T ref);
  T raw() const;
  T leak();

  bool operator!();

private:
  static T new_ref(T ref);

  T _ref;
};

template <typename T>
inline bool operator==(const local_ref<T> &ref, std::nullptr_t);

template <typename T>
inline bool operator==(std::nullptr_t, const local_ref<T> &ref);

template <typename T>
inline bool operator!=(const local_ref<T> &ref, std::nullptr_t);

template <typename T>
inline bool operator!=(std::nullptr_t, const local_ref<T> &ref);

template <typename T>
inline local_ref<T> make_local_ref(T ref);

//
// global_ref
//

template <typename T> class global_ref {

  static_assert(std::is_convertible<T, raw::object_ref>::value,
                "T must be convertible to jni::raw::object_ref");

public:
  enum steal_t { adopt_existing_global_ref };

  global_ref();
  global_ref(std::nullptr_t);
  explicit global_ref(T ref);
  global_ref(T ref, steal_t);
  global_ref(const global_ref &other);
  global_ref(global_ref &&other);

  template <typename U> global_ref(const local_ref<U> &other);
  template <typename U> explicit global_ref(local_ref<U> &&other);

  ~global_ref();

  void release();

  global_ref &operator=(const global_ref &other);
  global_ref &operator=(global_ref &&other);
  global_ref &operator=(const local_ref<T> &other);
  global_ref &operator=(local_ref<T> &&other);

  bool operator!();

  void assign(T ref);
  T raw() const;
  T leak();

private:
  static T new_ref(T ref);

  T _ref;
};

template <typename T>
inline bool operator==(const global_ref<T> &ref, std::nullptr_t);

template <typename T>
inline bool operator==(std::nullptr_t, const global_ref<T> &ref);

template <typename T>
inline bool operator!=(const global_ref<T> &ref, std::nullptr_t);

template <typename T>
inline bool operator!=(std::nullptr_t, const global_ref<T> &ref);

} // namespace jni

#define REF_INCLUDING
#include <jni/impl/ref.ipp>
#undef REF_INCLUDING
