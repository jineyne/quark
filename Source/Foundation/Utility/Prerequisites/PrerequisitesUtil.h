#pragma once

#include "PlatformDefines.h"

#if defined (DEBUG) || defined (_DEBUG)
#   define DEBUG_MODE 1
#else
#   define DEBUG_MODE 0
#endif

#include "StdHeaders.h"
#include "CommonTypes.h"

#if COMPILER == COMPILER_MSVC
#   define WIDEN2(EXP) L##EXP
#   define WIDEN(EXP) WIDEN2(EXP)
#   define WIDECHAR wchar_t
#   define ANSICHAR char
#   if DEBUG_MODE
#       define FORCEINLINE inline

#   else
#       define FORCEINLINE inline __forceinline
#   endif
#   define FORCENOINLINE __declspec(noinline)
#else
#   define WIDECHAR wchar_t
#   define ANSICHAR char
#   if DEBUG_MODE
#       define FORCEINLINE inline
#   else
#       define FORCEINLINE inline __attribute__((always_inline))
#   endif
#   define FORCENOINLINE __attribute__ ((noinline))
#endif

#if defined(UNICODE) || defined(_UNICODE)
#   define PLATFORM_TCHAR_IS_CHAR16 1
#else
#   define PLATFORM_TCHAR_IS_CHAR16 0
#endif

#if !defined(TEXT)
// If we don't have a platform-specific define for the TEXT macro, define it now.
#   if !defined(TEXT) && !BUILD_DOCS
#       if PLATFORM_TCHAR_IS_UTF8CHAR
#           define TEXT_PASTE(x) UTF8TEXT(x)
#       else
#           define TEXT_PASTE(x) WIDETEXT(x)
#       endif
#       define TEXT(x) TEXT_PASTE(x)
#   endif

using TCHAR = WIDECHAR;
#endif


#include "Memory/MemoryAllocator.h"
#include "Misc/AssertionMacros.h"
