#pragma once

#include "Win32.h"
#include "UtilityCore.h"
#include "Misc/Platform.h"

class DLL_EXPORT FWin32Platform : public FPlatform {
public:
    static void WndUpdate();
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};