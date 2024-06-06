//
// Created by jiney on 2024-06-01.
//

#include "ImGuiRendererExtension.h"

#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include <Manager/SceneManager.h>

#undef BOOL

#if WIN32
#include <windows.h>
#endif

ImGuiRendererExtension::ImGuiRendererExtension()
    : RendererExtension(ERenderLocation::Overlay, 1) {

}

void ImGuiRendererExtension::initialize(const std::any &data) {
    RendererExtension::initialize(data);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    auto primaryWindow = gCoreApplication().getPrimaryWindow();
    HWND hWnd = (HWND) primaryWindow->getData(TEXT("handle"));
    ID3D11Device *device = (ID3D11Device *)  primaryWindow->getData(TEXT("device"));
    ID3D11DeviceContext *deviceContext = (ID3D11DeviceContext *)  primaryWindow->getData(TEXT("deviceContext"));

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(device, deviceContext);
}

void ImGuiRendererExtension::destroy() {
    RendererExtension::destroy();

    ImGui::DestroyContext();
}

void ImGuiRendererExtension::render(CameraBase *camera) {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    gSceneManager().updateGui();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool ImGuiRendererExtension::checkCamera(CameraBase *camera) {
    return true;
}
