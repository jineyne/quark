#pragma once

#include "DX11Prerequisites.h"
#include "Manager/RenderStateManager.h"
#include "DX11RenderStateManager.g.h"

QCLASS()
class DX11_EXPORT DX11RenderStateManager : public RenderStateManager {
    GENERATED_BODY()

protected:
    SamplerState *createSamplerStateInternal(const SamplerStateDesc &desc) const override;
    DepthStencilState *createDepthStencilStateInternal(const DepthStencilStateDesc &desc) const override;
};
