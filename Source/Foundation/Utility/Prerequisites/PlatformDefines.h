#pragma once

#define PLATFORM_WIN32      1
#define PLATFORM_LINUX      2
#define PLATFORM_MACOS      3
#define PLATFORM_ANDROID    4
#define PLATFORM_IOS        5

#define COMPILER_MSVC       1
#define COMPILER_GNUC       2
#define COMPILER_CLANG      3

#define IS_TOUCH_PLATFORM    (PLATFORM == PLATFORM_ANDROID)


#if defined (_MSC_VER)
#   define COMPILER             COMPILER_MSVC
#   define THREADLOCAL          __declspec(thread)
#   define NORREF
#   define __PRETTY_FUNCTION__  __FUNCSIG__
#elif defined (__GNUC__)
#   define COMPILER             COMPILER_GNUC
#   define THREADLOCAL          __thread
#   define NORREF
#elif defined (__clang__)
#   define COMPILER             COMPILER_CLANG
#   define THREADLOCAL          __thread
#   define NORREF __attribute__((annotate("norref")))
#else
#   pragma error "Unable to find support compiler"
#endif

#if defined (ANDROID)
#   define PLATFORM             PLATFORM_ANDROID
#   define PLATFORM_NAME        Android
#elif defined (__WIN32__) || defined (_WIN32)
#   define PLATFORM             PLATFORM_WIN32
#   define PLATFORM_NAME        Desktop
#elif defined (__APPLE__)
#   define PLATFORM             PLATFORM_MACOS
#   define PLATFORM_NAME        Desktop
#else
#   define PLATFORM             PLATFORM_LINUX
#   define PLATFORM_NAME        Desktop
#endif

#if PLATFORM == PLATFORM_WIN32
#   if COMPILER == COMPILER_MSVC
#       if defined(EXPORTS)
#           define DLL_EXPORT __declspec(dllexport)
#       else
#           define DLL_EXPORT __declspec(dllimport)
#       endif
#   else
#       if defined(EXPORTS)
#           define DLL_EXPORT __attribute__ ((dllexport))
#       else
#           define DLL_EXPORT __attribute__ ((dllimport))
#       endif
#   endif
#else
#   define DLL_EXPORT __attribute__ ((visibility("default")))
#endif

#if PLATFORM == PLATFORM_WIN32 // Windows
#   if COMPILER == COMPILER_MSVC
#       define PLUGIN_EXPORT __declspec(dllexport)
#   else
#       define PLUGIN_EXPORT __attribute__ ((dllexport))
#   endif
#else // Linux/Android settings
#   define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

#define UTF8TEXT_PASTE(x)  u8 ## x
#define UTF16TEXT_PASTE(x) u ## x
#if PLATFORM_WIDECHAR_IS_CHAR16
#define WIDETEXT_PASTE(x)  UTF16TEXT_PASTE(x)
#else
#define WIDETEXT_PASTE(x)  L ## x
#endif

#define WIDETEXT(str) WIDETEXT_PASTE(str)