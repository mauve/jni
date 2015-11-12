
#pragma once

#define JNI_WITH(JNI_FEATURE) (JNI_WITH_##JNI_FEATURE)

#ifdef JNI_WITH_USER_CONFIG
#  include "jni.user.config.hpp"
#endif

#ifdef JNI_BUILDING
#  define JNI_EXPORT __declspec(dllexport)
#  define JNI_IMPORT
#  define JNI_TEMPLATE_EXTERN
#else
#  define JNI_EXPORT __declspec(dllimport)
#  define JNI_IMPORT __declspec(dllimport)
#  define JNI_TEMPLATE_EXTERN extern
#endif

#ifndef JNI_WITH_TESTING
#  define JNI_WITH_TESTING 1
#endif
