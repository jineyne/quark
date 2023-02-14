#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _TCHAR_DEFINED
#include <windows.h>
#include <windowsx.h>

#include <clocale>

#undef TEXT

#include "UtilityCore.h"

DLL_EXPORT DECLARE_LOG_CATEGORY_EXTERN(LogWin32, All);
