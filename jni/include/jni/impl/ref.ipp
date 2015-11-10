
#pragma once

#ifndef REF_INCLUDING
#error "ref.ipp cannot be including directly"
#endif

namespace jni {

template <typename T> inline local_ref<T>::local_ref() : _ref(nullptr) {}

template <typename T>
inline local_ref<T>::local_ref(std::nullptr_t)
    : _ref(nullptr) {}

template <typename T> inline local_ref<T>::local_ref(T ref) : _ref(ref) {}

template <typename T>
inline local_ref<T>::local_ref(const local_ref &other)
    : _ref(new_ref(other._ref)) {}

template <typename T>
inline local_ref<T>::local_ref(local_ref &&other)
    : _ref(other.leak()) {}

template <typename T>
inline local_ref<T>::local_ref(global_ref<T> &&other)
    : _ref(new_ref(other.raw())) {
  other.release();
}

template <typename T> inline local_ref<T>::~local_ref() { release(); }

template <typename T> inline void local_ref<T>::release() {
  environment::current().delete_local_ref(_ref);
  _ref = nullptr;
}

template <typename T>
inline local_ref<T> &local_ref<T>::operator=(const local_ref &other) {
  release();
  _ref = new_ref(other._ref);
  return *this;
}

template <typename T>
inline local_ref<T> &local_ref<T>::operator=(local_ref &&other) {
  release();
  _ref = other.leak();
  return *this;
}

template <typename T>
inline local_ref<T> &local_ref<T>::operator=(const global_ref<T> &other) {
  release();
  _ref = new_ref(other.raw());
  return *this;
}

template <typename T>
inline local_ref<T> &local_ref<T>::operator=(global_ref<T> &&other) {
  release();
  _ref = new_ref(other.leak());
  return *this;
}

template <typename T>
template <typename U>
inline local_ref<U> local_ref<T>::cast() {
  return local_ref<U>{static_cast<U>(leak())};
}

template <typename T> inline void local_ref<T>::assign(T ref) {
  release();
  _ref = ref;
}

template <typename T> inline T local_ref<T>::raw() const { return _ref; }

template <typename T> inline T local_ref<T>::leak() {
  auto copy = _ref;
  _ref = nullptr;
  return copy;
}

template <typename T> inline bool local_ref<T>::operator!() { return !_ref; }

template <typename T> inline T local_ref<T>::new_ref(T ref) {
  return static_cast<T>(environment::current().new_local_ref(ref));
}

template <typename T>
inline bool operator==(const local_ref<T> &ref, std::nullptr_t) {
  return ref.raw() == nullptr;
}
template <typename T>
inline bool operator==(std::nullptr_t, const local_ref<T> &ref) {
  return ref.raw() == nullptr;
}
template <typename T>
inline bool operator!=(const local_ref<T> &ref, std::nullptr_t) {
  return ref.raw() != nullptr;
}
template <typename T>
inline bool operator!=(std::nullptr_t, const local_ref<T> &ref) {
  return ref.raw() != nullptr;
}

template <typename T> inline local_ref<T> make_local_ref(T ref) {
  return local_ref<T>{ref};
}

//
// global_ref
//

template <typename T> inline global_ref<T>::global_ref() : _ref(nullptr) {}

template <typename T>
inline global_ref<T>::global_ref(std::nullptr_t)
    : _ref(nullptr) {}

template <typename T>
inline global_ref<T>::global_ref(T ref)
    : _ref(new_ref(ref)) {}

template <typename T>
inline global_ref<T>::global_ref(T ref, steal_t)
    : _ref(ref) {}

template <typename T>
inline global_ref<T>::global_ref(const global_ref<T> &other)
    : _ref(new_ref(other._ref)) {}

template <typename T>
inline global_ref<T>::global_ref(global_ref &&other)
    : _ref(static_cast<T>(other._ref)) {
  other._ref = nullptr;
}

template <typename T>
template <typename U>
inline global_ref<T>::global_ref(const local_ref<U> &other)
    : _ref(new_ref(reinterpret_cast<T>(other.raw()))) {}

template <typename T>
template <typename U>
inline global_ref<T>::global_ref(local_ref<U> &&other)
    : _ref(new_ref(reinterpret_cast<T>(other.raw()))) {
  other.release();
}

template <typename T> inline global_ref<T>::~global_ref() { release(); }

template <typename T> inline void global_ref<T>::release() {
  environment::current().delete_global_ref(_ref);
  _ref = nullptr;
}

template <typename T>
inline global_ref<T> &global_ref<T>::operator=(const global_ref<T> &other) {
  release();
  _ref = new_ref(other._ref);
  return *this;
}

template <typename T>
inline global_ref<T> &global_ref<T>::operator=(global_ref<T> &&other) {
  release();
  _ref = other._ref;
  other._ref = nullptr;
  return *this;
}

template <typename T>
inline global_ref<T> &global_ref<T>::operator=(const local_ref<T> &other) {
  release();
  _ref = new_ref(other.raw());
  return *this;
}

template <typename T>
inline global_ref<T> &global_ref<T>::operator=(local_ref<T> &&other) {
  release();
  _ref = new_ref(other.raw());
  other.release();
  return *this;
}

template <typename T> inline bool global_ref<T>::operator!() { return !_ref; }

template <typename T>
template <typename U>
inline global_ref<U> global_ref<T>::cast() {
  return global_ref<U>{static_cast<U>(leak()),
                       global_ref<U>::adopt_existing_global_ref};
}

template <typename T> inline void global_ref<T>::assign(T ref) {
  release();
  _ref = ref;
}

template <typename T> inline T global_ref<T>::raw() const { return _ref; }

template <typename T> inline T global_ref<T>::leak() {
  auto copy = _ref;
  _ref = nullptr;
  return copy;
}

template <typename T> inline T global_ref<T>::new_ref(T ref) {
  return static_cast<T>(environment::current().new_global_ref(ref));
}

template <typename T>
inline bool operator==(const global_ref<T> &ref, std::nullptr_t) {
  return ref.raw() == nullptr;
}

template <typename T>
inline bool operator==(std::nullptr_t, const global_ref<T> &ref) {
  return ref.raw() == nullptr;
}

template <typename T>
inline bool operator!=(const global_ref<T> &ref, std::nullptr_t) {
  return ref.raw() != nullptr;
}

template <typename T>
inline bool operator!=(std::nullptr_t, const global_ref<T> &ref) {
  return ref.raw() != nullptr;
}

} // namespace jni
