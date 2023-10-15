#include "Texture.h"
#include "Manager/TextureManager.h"
#include "Resource/Resources.h"

DEFINE_LOG_CATEGORY(LogTexture)

FResourceHandle<Texture> Texture::White = nullptr;
FResourceHandle<Texture> Texture::Black = nullptr;

Texture::Texture(const TextureDesc &desc, FPixelData *initData) : mDesc(desc), mInitData(initData) { }

Texture::~Texture() { }

FResourceHandle<Texture> Texture::New(const TextureDesc &desc) {
    auto obj = TextureManager::Instance().createTexture(desc);
    return StaticResourceCast<Texture>(gResources().createResourceHandle(obj, false));
}

Texture *Texture::NewPtr(const TextureDesc &desc) {
    return TextureManager::Instance().createTexture(desc);
}

void Texture::writeData(FPixelData *data, uint32_t mipLevel, uint32_t face, bool discardWholeBuffer) {
    if (discardWholeBuffer) {
        if (mDesc.usage == ETextureUsage::Dynamic) {
            discardWholeBuffer = false;
        }
    }

    writeDataInternal(data, mipLevel, face, discardWholeBuffer);
}

FPixelData Texture::lock(EGpuLockOptions options, uint32_t mipLevel, uint32_t face) {
    if (mipLevel > mDesc.mipLevels) {
        LOG(LogTexture, Error, TEXT("Invalid mip level: %d. Min is 0, max is %d"), mipLevel, mDesc.mipLevels);
        return FPixelData(0, 0, 0, EPixelFormat::Unknown);
    }

    if (face >= getFaceCount()) {
        LOG(LogTexture, Error, TEXT("Invalid face level: %d. Min is 0, max is %d"), face, getFaceCount());
        return FPixelData(0, 0, 0, EPixelFormat::Unknown);
    }

    return lockInternal(options, mipLevel, face);
}

void Texture::unlock() {
    unlockInternal();
}

TextureView *Texture::requestView(uint32_t mostDetailMip, uint32_t mipCount, uint32_t firstArraySlice,
                                  uint32_t arraySlicesCount, EGpuViewUsage usage) {
    TextureViewDesc key;
    key.mostDetailMip = mostDetailMip;
    key.mipCount = mipCount == 0 ? (mDesc.mipLevels + 1) : mipCount;
    key.firstArraySlice = firstArraySlice;
    key.arraySlicesCount = arraySlicesCount == 0 ? mDesc.sampleCount : arraySlicesCount;
    key.usage = usage;

    auto it = mTextureViews.find(key);
    if (it == nullptr) {
        mTextureViews[key] = createView(key);

        it = mTextureViews.find(key);
    }

    return *it;
}

void Texture::initialize() {
    if (mInitData != nullptr) {
        writeData(mInitData, 0, 0, true);
        mInitData->unlock();
        mInitData = nullptr;
    }

    Resource::initialize();
}

TextureView *Texture::createView(const TextureViewDesc &desc) {
    auto data = q_alloc<TextureView>();
    return new (data) TextureView(desc);
}
