#include "DX11RenderAPIFactory.h"
#include "Misc/Module.h"
#include "DX11RenderAPI.h"

void DX11RenderAPIFactory::create() {
    RenderAPI::StartUp<DX11RenderAPI>();
}

DX11RenderAPIFactory::InitOnStart DX11RenderAPIFactory::initOnStart;