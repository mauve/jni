
#pragma once

#include <jni/raw/types.hpp>

namespace jni {

class environment;

namespace raw {

template <typename T> class field {
public:
  field(field_id fid);

  T read(environment &env, object_ref instance) const;
  void write(environment &env, object_ref instance, const T &value);

private:
  field_id _fid;
};

} // namespace raw
} // namespace jni

#define RAW_FIELD_INCLUDING
#include <jni/impl/raw/field.ipp>
#undef RAW_FIELD_INCLUDING
