
#pragma once

#define JNI_WITH(JNI_FEATURE) (JNI_WITH_##JNI_FEATURE)

#ifdef JNI_WITH_USER_CONFIG
#  include "jni.user.config.hpp"
#endif

#ifdef JNI_BUILDING
#  define JNI_EXPORT __declspec(dllexport)
#else
#  define JNI_EXPORT __declspec(dllimport)
#endif

#ifndef JNI_WITH_CHECKED_REFS
#  define JNI_WITH_CHECKED_REFS 0
#endif

#ifndef JNI_WITH_CHECKED_REFS_ASSERTS
#  if JNI_WITH_CHECKED_REFS
#    define JNI_WITH_CHECKED_REFS_ASSERTS 1
#  else
#    define JNI_WITH_CHECKED_REFS_ASSERTS 0
#  endif
#endif

#ifndef JNI_WITH_TESTING
#  define JNI_WITH_TESTING 1
#endif
