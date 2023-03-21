//
// Created by jiney on 2023-03-19.
//

#include "DX11TextureManager.h"
#include "Image/DX11Texture.h"

FTexture *FDX11TextureManager::createTextureInternal(const FTextureDesc &desc) {
    return new FDX11Texture(desc, nullptr);
}
