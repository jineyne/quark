#pragma once

#include "Win32.h"
#include "Misc/Platform.h"

struct IWindowMessageHandler : public IPlatformEventHandlerBase {
    virtual void preprocessMessage(HWND hWnd, uint32_t uMsg, WPARAM wParam, LPARAM lParam) {}
    virtual bool handleMessage(HWND hWnd, uint32_t uMsg, WPARAM wParam, LPARAM lParam, LRESULT* outResult) { return false; }
};