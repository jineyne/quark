#include "TextureManager.h"

FTexture *FTextureManager::createTexture(const FTextureDesc &desc) {
    auto texture = createTextureInternal(desc);
    texture->initialize();

    return texture;
}

void FTextureManager::onStartUp() {
    FTextureDesc desc{};
    desc.type = ETextureType::e2D;
    desc.width = 2;
    desc.height = 2;
    desc.format = EPixelFormat::RGBA8;
    desc.usage = ETextureUsage::Static;

    auto whiteTexture = createTexture(desc);
    auto whitePixelData = FPixelData::New(2, 2, 1, EPixelFormat::RGBA8);
    whitePixelData->setColors(FColor::White);
    whiteTexture->writeData(whitePixelData);
    FTexture::White = whiteTexture;

    auto blackTexture = createTexture(desc);
    auto blackPixelData = FPixelData::New(2, 2, 1, EPixelFormat::RGBA8);
    blackPixelData->setColors(FColor::Black);
    blackTexture->writeData(blackPixelData);
    FTexture::Black = blackTexture;
}

void FTextureManager::onShutDown() {
    q_delete(FTexture::Black);
    q_delete(FTexture::White);
}
