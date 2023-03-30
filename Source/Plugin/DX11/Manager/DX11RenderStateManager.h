#pragma once

#include "DX11Prerequisites.h"
#include "Manager/RenderStateManager.h"
#include "DX11RenderStateManager.g.h"

QCLASS()
class DX11_EXPORT FDX11RenderStateManager : public FRenderStateManager {
    GENERATED_BODY()

protected:
    FSamplerState *createSamplerStateInternal(const FSamplerStateDesc &desc) const override;
    FDepthStencilState *createDepthStencilStateInternal(const FDepthStencilStateDesc &desc) const override;
};
