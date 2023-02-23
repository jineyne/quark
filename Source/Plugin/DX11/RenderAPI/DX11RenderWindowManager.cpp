#include "DX11RenderWindowManager.h"
#include "RenderAPi/RenderAPI.h"
#include "DX11RenderAPI.h"
#include "DX11RenderWindow.h"

FRenderWindow *FDX11RenderWindowManager::createInternal(const FRenderWindowDesc &desc, uint32_t windowId, FRenderWindow *parent) {
    FRenderAPI *rapi = FRenderAPI::InstancePtr();
    FDX11RenderAPI *dx11api = static_cast<FDX11RenderAPI *>(rapi);

    return new FDX11RenderWindow(desc, windowId, dx11api->getPrimaryDevice(), dx11api->getDXGIFactory(), parent);
}
