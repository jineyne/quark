#pragma once

#include "DX11Prerequisites.h"
#include "Manager/TextureManager.h"

class DX11_EXPORT FDX11TextureManager : public FTextureManager {
protected:
    FTexture *createTextureInternal(const FTextureDesc &desc) override;
};