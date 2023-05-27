#include "DX11RenderWindowManager.h"
#include "RenderAPI/RenderAPI.h"
#include "Win32/Win32Platform.h"
#include "RenderAPI/DX11RenderAPI.h"
#include "RenderAPI/DX11RenderWindow.h"

void DX11RenderWindowManager::update() {
    RenderWindowManager::update();

    Win32Platform::WndUpdate();
};

RenderWindow *DX11RenderWindowManager::createInternal(const RenderWindowDesc &desc, uint32_t windowId, RenderWindow *parent) {
    RenderAPI *rapi = RenderAPI::InstancePtr();
    DX11RenderAPI *dx11api = static_cast<DX11RenderAPI *>(rapi);

    return q_new<DX11RenderWindow>(desc, windowId, dx11api->getPrimaryDevice(), dx11api->getDXGIFactory(), parent);
}
