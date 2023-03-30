#include "DX11RenderStateManager.h"
#include "RenderAPI/DX11SamplerState.h"
#include "RenderAPI/DX11DepthStencilState.h"

FSamplerState *FDX11RenderStateManager::createSamplerStateInternal(const FSamplerStateDesc &desc) const {
    return new (q_alloc<FDX11SamplerState>()) FDX11SamplerState(desc);
}

FDepthStencilState *FDX11RenderStateManager::createDepthStencilStateInternal(const FDepthStencilStateDesc &desc) const {
    return new (q_alloc<FDX11DepthStencilState>()) FDX11DepthStencilState(desc);
}
