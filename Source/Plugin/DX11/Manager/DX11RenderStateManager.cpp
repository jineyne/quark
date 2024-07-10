#include "DX11RenderStateManager.h"

#include "RenderAPI/DX11BlendState.h"
#include "RenderAPI/DX11RasterizerState.h"
#include "RenderAPI/DX11SamplerState.h"
#include "RenderAPI/DX11DepthStencilState.h"


BlendState *DX11RenderStateManager::createBlendStateInternal(const BlendStateDesc &desc) const {
    return new (q_alloc<DX11BlendState>()) DX11BlendState(desc, 0);
}

SamplerState *DX11RenderStateManager::createSamplerStateInternal(const SamplerStateDesc &desc) const {
    return new (q_alloc<DX11SamplerState>()) DX11SamplerState(desc);
}

DepthStencilState *DX11RenderStateManager::createDepthStencilStateInternal(const DepthStencilStateDesc &desc) const {
    return new (q_alloc<DX11DepthStencilState>()) DX11DepthStencilState(desc);
}

RasterizerState *DX11RenderStateManager::createRasterizerStateInternal(const RasterizerStateDesc &desc) const {
    return new (q_alloc<DX11RasterizerState>()) DX11RasterizerState(desc);
};