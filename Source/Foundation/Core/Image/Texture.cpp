#include "Texture.h"
#include "Manager/TextureManager.h"
#include "Resource/Resources.h"

DEFINE_LOG_CATEGORY(FLogTexture)

FTexture *FTexture::White = nullptr;
FTexture *FTexture::Black = nullptr;

FTexture::FTexture(const FTextureDesc &desc, FPixelData *initData) : mDesc(desc), mInitData(initData) { }

FTexture::~FTexture() { }

FResourceHandle<FTexture> FTexture::New(const FTextureDesc &desc) {
    auto obj = FTextureManager::Instance().createTexture(desc);
    return StaticResourceCast<FTexture>(gResources().createResourceHandle(obj, false));
}

void FTexture::writeData(FPixelData *data, uint32_t mipLevel, uint32_t face, bool discardWholeBuffer) {
    if (discardWholeBuffer) {
        if (mDesc.usage == ETextureUsage::Dynamic) {
            discardWholeBuffer = false;
        }
    }

    writeDataInternal(data, mipLevel, face, discardWholeBuffer);
}

FPixelData FTexture::lock(EGpuLockOptions options, uint32_t mipLevel, uint32_t face) {
    if (mipLevel > mDesc.mipLevels) {
        LOG(FLogTexture, Error, TEXT("Invalid mip level: %d. Min is 0, max is %d"), mipLevel, mDesc.mipLevels);
        return FPixelData(0, 0, 0, EPixelFormat::Unknown);
    }

    if (face >= getFaceCount()) {
        LOG(FLogTexture, Error, TEXT("Invalid face level: %d. Min is 0, max is %d"), face, getFaceCount());
        return FPixelData(0, 0, 0, EPixelFormat::Unknown);
    }

    return lockInternal(options, mipLevel, face);
}

void FTexture::unlock() {
    unlockInternal();
}

FTextureView *FTexture::requestView(uint32_t mostDetailMip, uint32_t mipCount, uint32_t firstArraySlice,
                                    uint32_t arraySlicesCount, EGpuViewUsage usage) {
    FTextureViewDesc key;
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

void FTexture::initialize() {
    if (mInitData != nullptr) {
        writeData(mInitData, 0, 0, true);
        mInitData->unlock();
        mInitData = nullptr;
    }

    FResource::initialize();
}

FTextureView *FTexture::createView(const FTextureViewDesc &desc) {
    auto data = q_alloc<FTextureView>();
    return new (data) FTextureView(desc);
}
