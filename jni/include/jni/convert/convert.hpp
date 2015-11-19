
#pragma once

namespace jni {
namespace convert {

template <typename Target, typename Source> struct to_java {
  to_java(const Source &src);

  Target convert();
};

template <typename Target, typename Source> struct from_java {
  from_java(const Source &src);

  Target convert();
};

} // namespace convert

template <typename Target, typename Source>
inline Target to_java(const Source &src) {
  convert::to_java<Target, Source> converter{src};

  return converter.convert();
}

template <typename Target, typename Source>
inline Target from_java(const Source &src) {
  convert::from_java<Target, Source> converter{src};

  return converter.convert();
}

} // namespace jni
