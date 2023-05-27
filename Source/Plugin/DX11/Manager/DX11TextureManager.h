#pragma once

#include "DX11Prerequisites.h"
#include "Manager/TextureManager.h"
#include "DX11TextureManager.g.h"

QCLASS()
class DX11_EXPORT DX11TextureManager : public TextureManager {
    GENERATED_BODY()

protected:
    Texture *createTextureInternal(const TextureDesc &desc) override;
};