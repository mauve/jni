
#pragma once

#include <jni/class_name.hpp>
#include <jni/global_ref.hpp>
#include <jni/java/lang/object.hpp>
#include <jni/java/lang/class.hpp>

namespace jni {

template <typename Target> Target object_cast(const java::lang::Object &obj) {
  auto target_name = jni::class_name<Target>::name();
  auto target_cls = java::lang::Class::forName(target_name);
  auto source_cls = obj.getClass();

  if (!target_cls.isAssignableFrom(source_cls)) {
    // TODO: implement ClassCastException
    throw std::bad_cast("cannot cast " + source_cls.getName() +
                        " to " target_cls.getName());
  }

  global_ref<raw::object_ref> ref_copy{obj._ref};
  return Target{std::move(ref_copy)};
}

} // namespace jni
