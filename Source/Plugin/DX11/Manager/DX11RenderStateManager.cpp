#include "DX11RenderStateManager.h"
#include "RenderAPI/DX11SamplerState.h"
#include "RenderAPI/DX11DepthStencilState.h"

SamplerState *DX11RenderStateManager::createSamplerStateInternal(const SamplerStateDesc &desc) const {
    return new (q_alloc<DX11SamplerState>()) DX11SamplerState(desc);
}

DepthStencilState *DX11RenderStateManager::createDepthStencilStateInternal(const DepthStencilStateDesc &desc) const {
    return new (q_alloc<DX11DepthStencilState>()) DX11DepthStencilState(desc);
}
