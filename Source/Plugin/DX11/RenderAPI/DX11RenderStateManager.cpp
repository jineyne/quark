#include "DX11RenderStateManager.h"

FSamplerState *FDX11RenderStateManager::createSamplerStateInternal(const FSamplerStateDesc &desc) const {
    return FRenderStateManager::createSamplerStateInternal(desc);
}
