#include "DX11RenderWindowManager.h"
#include "RenderAPI/RenderAPI.h"
#include "Win32/Win32Platform.h"
#include "RenderAPI/DX11RenderAPI.h"
#include "RenderAPI/DX11RenderWindow.h"

void FDX11RenderWindowManager::update() {
    FRenderWindowManager::update();

    FWin32Platform::WndUpdate();
};

FRenderWindow *FDX11RenderWindowManager::createInternal(const FRenderWindowDesc &desc, uint32_t windowId, FRenderWindow *parent) {
    FRenderAPI *rapi = FRenderAPI::InstancePtr();
    FDX11RenderAPI *dx11api = static_cast<FDX11RenderAPI *>(rapi);

    return new FDX11RenderWindow(desc, windowId, dx11api->getPrimaryDevice(), dx11api->getDXGIFactory(), parent);
}
