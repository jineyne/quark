#pragma once

#include "ImGuiPrerequisites.h"

#undef BOOL

#include <Win32/IWindowMessageHandler.h>
#include <Manager/InputManager.h>

class PLUGIN_EXPORT ImGuiInputHandler : public IWindowMessageHandler {
public:
    void preprocessMessage(HWND hWnd, uint32_t uMsg, WPARAM wParam, LPARAM lParam) override;
    bool handleMessage(HWND hWnd, uint32_t uMsg, WPARAM wParam, LPARAM lParam, LRESULT *outResult) override;
};
