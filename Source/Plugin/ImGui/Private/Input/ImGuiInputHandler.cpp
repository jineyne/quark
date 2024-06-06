//
// Created by jiney on 2024-06-03.
//

#include "ImGuiInputHandler.h"

#include <backends/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void ImGuiInputHandler::preprocessMessage(HWND hWnd, uint32_t uMsg, WPARAM wParam, LPARAM lParam) {
}

bool ImGuiInputHandler::handleMessage(HWND hWnd, uint32_t uMsg, WPARAM wParam, LPARAM lParam, LRESULT *outResult) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
        *outResult = true;
    }

    return false;
}
