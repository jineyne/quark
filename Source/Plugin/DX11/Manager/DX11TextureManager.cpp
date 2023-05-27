//
// Created by jiney on 2023-03-19.
//

#include "DX11TextureManager.h"
#include "Image/DX11Texture.h"

Texture *DX11TextureManager::createTextureInternal(const TextureDesc &desc) {
    return new DX11Texture(desc, nullptr);
}
