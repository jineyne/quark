#include "TextureManager.h"

Texture *TextureManager::createTexture(const TextureDesc &desc) {
    auto texture = createTextureInternal(desc);
    texture->initialize();

    return texture;
}

void TextureManager::onStartUp() {
    TextureDesc desc{};
    desc.type = ETextureType::e2D;
    desc.width = 2;
    desc.height = 2;
    desc.format = EPixelFormat::RGBA8;
    desc.usage = ETextureUsage::Static;

    auto whiteTexture = createTexture(desc);
    auto whitePixelData = FPixelData::New(2, 2, 1, EPixelFormat::RGBA8);
    whitePixelData->setColors(Color::White);
    whiteTexture->writeData(whitePixelData);
    Texture::White = whiteTexture;

    auto blackTexture = createTexture(desc);
    auto blackPixelData = FPixelData::New(2, 2, 1, EPixelFormat::RGBA8);
    blackPixelData->setColors(Color::Black);
    blackTexture->writeData(blackPixelData);
    Texture::Black = blackTexture;
}

void TextureManager::onShutDown() {
    q_delete(Texture::Black);
    q_delete(Texture::White);
}
