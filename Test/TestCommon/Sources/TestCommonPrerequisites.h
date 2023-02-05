#pragma once

#include "UtilityCore.h"

#if PLATFORM == PLATFORM_WIN32
#   if COMPILER == COMPILER_MSVC
#       if defined(TestCommon_EXPORTS)
#           define TestCommon_EXPORT __declspec(dllexport)
#       else
#           define TestCommon_EXPORT __declspec(dllimport)
#       endif
#   else
#       if defined(TestCommon_EXPORTS)
#           define TestCommon_EXPORT __attribute__ ((dllexport))
#       else
#           define TestCommon_EXPORT __attribute__ ((dllimport))
#       endif
#   endif
#else
#   define TestCommon_EXPORT __attribute__ ((visibility("default")))
#endif