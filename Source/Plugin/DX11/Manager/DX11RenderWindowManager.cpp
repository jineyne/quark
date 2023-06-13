#include "DX11RenderWindowManager.h"
#include "RenderAPI/RenderAPI.h"
#include "Win32/Win32Platform.h"
#include "RenderAPI/DX11RenderAPI.h"
#include "RenderAPI/DX11RenderWindow.h"
#include "CoreApplication.h"

void DX11RenderWindowManager::update() {
    RenderWindowManager::update();

    Win32Platform::WndUpdate();
};

bool DX11RenderWindowManager::handleMessage(HWND hWnd, uint32_t uMsg, WPARAM wParam, LPARAM lParam, LRESULT *outResult) {
    switch (uMsg) {
        case WM_QUIT:
        case WM_CLOSE:
            gCoreApplication().quitRequest();
            *outResult = 0;
            return true;
    }

    return false;
}

void DX11RenderWindowManager::onStartUp() {
    Super::onStartUp();

    Platform::RegisterPlatformEventHandler(this);
}

void DX11RenderWindowManager::onShutDown() {
    Platform::UnRegisterPlatformEventHandler(this);

    Super::onShutDown();
}


RenderWindow *DX11RenderWindowManager::createInternal(const RenderWindowDesc &desc, uint32_t windowId, RenderWindow *parent) {
    RenderAPI *rapi = RenderAPI::InstancePtr();
    DX11RenderAPI *dx11api = static_cast<DX11RenderAPI *>(rapi);

    return q_new<DX11RenderWindow>(desc, windowId, dx11api->getPrimaryDevice(), dx11api->getDXGIFactory(), parent);
}
