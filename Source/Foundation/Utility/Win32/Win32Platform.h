#pragma once

#include "Win32.h"
#include "UtilityCore.h"
#include "Misc/Platform.h"

class DLL_EXPORT Win32Platform : public Platform {
public:
    static void WndUpdate();
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};