#include "DX11RenderAPIFactory.h"
#include "Misc/Module.h"
#include "DX11RenderAPI.h"

void FDX11RenderAPIFactory::create() {
    FRenderAPI::StartUp<FDX11RenderAPI>();
}

FDX11RenderAPIFactory::InitOnStart FDX11RenderAPIFactory::initOnStart;