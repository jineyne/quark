#pragma once

#include "DX11Prerequisites.h"
#include "Image/Texture.h"
#include "Image/PixelData.h"

class DX11_EXPORT FDX11Texture  : public FTexture  {
private:
    ID3D11Texture2D *mTex2D;
    ID3D11Resource *mTexture;
    ID3D11ShaderResourceView *mView;

    uint32_t mLockedSubresourceIdx = -1;

    ID3D11Resource *mStagingBuffer = nullptr;
    FPixelData *mStaticBuffer = nullptr;

    bool mLockedForReading = false;

public:
    FDX11Texture(const FTextureDesc &desc, FPixelData *initData);
    virtual ~FDX11Texture();

public:
    ID3D11Texture2D *getTex2D() const { return mTex2D; }
    ID3D11Resource *getTexture() const { return mTexture; }
    ID3D11ShaderResourceView *getView() const { return mView; }

protected:
    void initialize() override;

    void createStagingBuffer();

    FPixelData lockInternal(EGpuLockOptions options, uint32_t mipLevel, uint32_t face) override;
    void unlockInternal() override;

    void writeDataInternal(FPixelData *data, uint32_t mipLevel, uint32_t face, bool discardWholeBuffer) override;

    void *map(ID3D11Resource *res, D3D11_MAP flags, uint32_t mipLevel, uint32_t face, uint32_t &rowPitch, uint32_t &slicePitch);
    void unmap(ID3D11Resource *res);

    void *mapstagingbuffer(D3D11_MAP flags, uint32_t mipLevel, uint32_t face, uint32_t& rowPitch, uint32_t& slicePitch);
    void unmapstagingbuffer();

    void *mapstaticbuffer(FPixelData lock, uint32_t mipLevel, uint32_t face);
    void unmapstaticbuffer();
};
