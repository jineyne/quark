#pragma once

#include "CorePrerequisites.h"
#include "PixelUtil.h"
#include "Resource/Resource.h"
#include "PixelData.h"
#include "RenderAPI/TextureView.h"

DLL_EXPORT DECLARE_LOG_CATEGORY_EXTERN(FLogTexture, Debug)

struct FTextureDesc {
    ETextureType type = ETextureType::e2D;
    ETextureUsage usage = ETextureUsage::Default;
    EPixelFormat format = EPixelFormat::RGBA8;

    uint32_t width = 1;
    uint32_t height = 1;
    uint32_t depth = 1;
    uint32_t mipLevels = 0;

    bool hwGamma = false;
    uint32_t sampleCount = 0;
    uint32_t arraySlicesCount = 1;
};

class DLL_EXPORT FTexture : public FResource  {

public:
    static FTexture *White;
    static FTexture *Black;

protected:
    FTextureDesc mDesc;
    FPixelData *mInitData;

    TMap<FTextureViewDesc, FTextureView *, FTextureView::HashFunction, FTextureView::EqualFunction> mTextureViews;

    bool mIsLock;

public:
    FTexture(const FTextureDesc &desc, FPixelData *initData);
    ~FTexture();

public:
    static FResourceHandle<FTexture> New(const FTextureDesc &desc);

public:
    void writeData(FPixelData *data, uint32_t mipLevel = 0, uint32_t face = 0,
                   bool discardWholeBuffer = false);

    FPixelData lock(EGpuLockOptions options, uint32_t mipLevel = 0, uint32_t face = 0);
    void unlock();

    FTextureView *requestView(uint32_t mostDetailMip, uint32_t mipCount, uint32_t firstArraySlice,
                              uint32_t arraySlicesCount, EGpuViewUsage usage);

    uint32_t getWidth() const { return mDesc.width; }
    uint32_t getHeight() const { return mDesc.height; }
    uint32_t getDepth() const { return mDesc.depth; }
    uint32_t getFaceCount() const { return mDesc.arraySlicesCount; }
    uint32_t getSampleCount() const { return mDesc.sampleCount; }
    EPixelFormat getFormat() const { return mDesc.format; }
    bool isHardwareGammaEnabled() const { return mDesc.hwGamma; }
    const FTextureDesc &getDesc() const { return mDesc; }

protected:
    void initialize() override;

    virtual FPixelData lockInternal(EGpuLockOptions options, uint32_t mipLevel = 0, uint32_t face = 0) = 0;
    virtual void unlockInternal() = 0;
    virtual void writeDataInternal(FPixelData *data, uint32_t mipLevel = 0, uint32_t face = 0,
                                   bool discardWholeBuffer = false) = 0;

    virtual FTextureView *createView(const FTextureViewDesc& desc);

private:
    friend class FTextureManager;
};