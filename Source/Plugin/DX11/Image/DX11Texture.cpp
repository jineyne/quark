#include "DX11Texture.h"
#include "RenderAPI/DX11Device.h"
#include "RenderAPI/DX11RenderAPI.h"
#include "Utility/DX11Mapper.h"

FDX11Texture::FDX11Texture(const FTextureDesc &desc, FPixelData *initData) : FTexture(desc, initData) {

}

FDX11Texture::~FDX11Texture() {
    SAFE_RELEASE(mTexture);
    SAFE_RELEASE(mTex2D);
    SAFE_RELEASE(mStagingBuffer);
}

void FDX11Texture::initialize() {
    FTexture::initialize();

    auto width = mDesc.width;
    auto height = mDesc.height;
    auto depth = mDesc.depth;
    auto type = mDesc.type;

    ETextureUsage usage = mDesc.usage;
    auto mipCount = mDesc.mipLevels;
    auto faceCount = getFaceCount();

    auto sampleCount = mDesc.sampleCount;

    width = std::max(width, 1U);
    height = std::max(height, 1U);

    DXGI_FORMAT format = FDX11Mapper::Get(mDesc.format);

    auto *rapi = static_cast<FDX11RenderAPI *>(FRenderAPI::InstancePtr());
    auto device = rapi->getPrimaryDevice();

    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = mipCount;
    desc.ArraySize = faceCount == 0 ? 1 : faceCount;
    desc.Format = format;
    desc.MiscFlags = 0;

    if ((usage & ETextureUsage::DepthStencil) == ETextureUsage::DepthStencil) {
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        DXGI_SAMPLE_DESC sampleDesc;
        rapi->determineMultisampleSettings(sampleCount, format, &sampleDesc);
        desc.SampleDesc = sampleDesc;
    } else {
        desc.Usage = FDX11Mapper::GetUsage((EBufferUsage) mDesc.usage);
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = FDX11Mapper::GetAccessFlags((EBufferUsage) mDesc.usage);

        DXGI_SAMPLE_DESC sampleDesc;
        sampleDesc.Count = 1;
        sampleDesc.Quality = 0;
        desc.SampleDesc = sampleDesc;
    }

    desc.MipLevels = (mipCount == 0x7FFFFFFF || (1U << mipCount) > width) ? 0 : mipCount + 1;

    if ((usage & ETextureUsage::LoadStore) == ETextureUsage::LoadStore) {
        if (desc.SampleDesc.Count <= 1) {
            desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
        } else {
            LOG(FLogTexture, Warning, TEXT("Unable to create a load-store texture with multiple samples. This is not ")
                                      TEXT("supported on DirectX 11. Ignoring load-store usage flag."));
        }
    }

    HRESULT hr = device->getDevice()->CreateTexture2D(&desc, nullptr, &mTex2D);

    if (FAILED(hr) || device->hasError()) {
        LOG(FLogTexture, Fatal, TEXT("Could not create texture: %ls"), *device->getErrorDescription());
        return;
    }

    hr = mTex2D->QueryInterface(__uuidof(ID3D11Resource), (void **) &mTexture);
    if (FAILED(hr) || device->hasError()) {
        LOG(FLogTexture, Fatal, TEXT("Could not get texture base: %ls"), *device->getErrorDescription());
        return;
    }

    mTex2D->GetDesc(&desc);
    if (mipCount != (desc.MipLevels - 1)) {
        EXCEPT(FLogTexture, RenderAPIException, TEXT("Driver returned different number of mip maps than requested. ")
                                                TEXT("Required: %d. Got: %d."), mipCount, desc.MipLevels - 1);
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc{};
    resourceViewDesc.Format = desc.Format;
    resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resourceViewDesc.Texture2D.MostDetailedMip = 0;
    resourceViewDesc.Texture2D.MipLevels = desc.MipLevels;

    hr = device->getDevice()->CreateShaderResourceView(mTexture, &resourceViewDesc, &mView);
    if (FAILED(hr) || device->hasError()) {
        LOG(FLogTexture, Fatal, TEXT("Could not create shader resource view: %ls"), *device->getErrorDescription());
    }
}

void FDX11Texture::createStagingBuffer() {
    auto *rapi = static_cast<FDX11RenderAPI *>(FRenderAPI::InstancePtr());
    auto device = rapi->getPrimaryDevice();

    switch (mDesc.type) {
        case ETextureType::e2D:
            D3D11_TEXTURE2D_DESC desc;
            mTex2D->GetDesc(&desc);

            desc.BindFlags = 0;
            desc.MiscFlags = 0;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
            desc.Usage = D3D11_USAGE_STAGING;

            device->getDevice()->CreateTexture2D(&desc, nullptr, (ID3D11Texture2D**)(&mStagingBuffer));
            break;
    }
}

FPixelData FDX11Texture::lockInternal(EGpuLockOptions options, uint32_t mipLevel, uint32_t face) {
    if (mDesc.sampleCount > 1) {
        EXCEPT(FLogTexture, InvalidStateException, TEXT("Multisampled textures cannot be accessed from the CPU directly."));
    }

    auto mipWidth = std::max(1u, mDesc.width >> mipLevel);
    auto mipHeight = std::max(1u, mDesc.height >> mipLevel);
    auto mipDepth = std::max(1u, mDesc.depth >> mipLevel);

    FPixelData lockedArea(mipWidth, mipHeight, mipDepth, mDesc.format);

    D3D11_MAP flags = FDX11Mapper::GetLockOptions((EGpuLockOptions) options);
    uint32_t rowPitch, slicePitch;

    if(flags == D3D11_MAP_READ || flags == D3D11_MAP_READ_WRITE) {
        uint8_t* data = (uint8_t *)mapstagingbuffer(flags, face, mipLevel, rowPitch, slicePitch);
        lockedArea.setExternalBuffer(data);
        lockedArea.setRowPitch(rowPitch);
        lockedArea.setSlicePitch(slicePitch);

        mLockedForReading = true;
    } else {
        if (mDesc.usage == ETextureUsage::Dynamic) {
            if(flags == D3D11_MAP_WRITE) {
                LOG(FLogTexture, Error, TEXT("Dynamic textures only support discard or no-overwrite writes. Falling back to no-overwrite."));
                flags = D3D11_MAP_WRITE_DISCARD;
            }

            uint8_t* data = (uint8_t*) map(mTexture, flags, face, mipLevel, rowPitch, slicePitch);
            lockedArea.setExternalBuffer(data);
            lockedArea.setRowPitch(rowPitch);
            lockedArea.setSlicePitch(slicePitch);
        } else {
            lockedArea.setExternalBuffer((uint8_t*) mapstaticbuffer(lockedArea, mipLevel, face));
        }

        mLockedForReading = false;
    }

    return lockedArea;
}

void FDX11Texture::unlockInternal() {
    if(mLockedForReading) {
        unmapstagingbuffer();
    } else {
        if (mDesc.usage == ETextureUsage::Dynamic) {
            unmap(mTexture);
        } else {
            unmapstaticbuffer();
        }
    }
}

void FDX11Texture::writeDataInternal(FPixelData *data, uint32_t mipLevel, uint32_t face, bool discardWholeBuffer) {
    auto *rapi = static_cast<FDX11RenderAPI *>(FRenderAPI::InstancePtr());
    ID3D11DeviceContext *context = rapi->getPrimaryDevice()->getImmediateContext();
    uint32_t rowWidth = FDX11Mapper::GetSizeInBytes(mDesc.format, mDesc.width);
    uint32_t sliceWidth = FDX11Mapper::GetSizeInBytes(mDesc.format, mDesc.width, mDesc.height);

    context->UpdateSubresource(mTexture, 0, nullptr, data->getData(), rowWidth, sliceWidth);
}

void *FDX11Texture::map(ID3D11Resource *res, D3D11_MAP flags, uint32_t mipLevel, uint32_t face, uint32_t &rowPitch,
                        uint32_t &slicePitch) {
    D3D11_MAPPED_SUBRESOURCE pMappedResource;
    pMappedResource.pData = nullptr;

    mipLevel = std::clamp(mipLevel, (UINT32) mipLevel, mDesc.mipLevels);
    face = std::clamp(face, (UINT32) 0, mDesc.arraySlicesCount - 1);

    // if (mDesc.type == TextureType::e3D) face = 0;

    auto *rapi = static_cast<FDX11RenderAPI *>(FRenderAPI::InstancePtr());
    auto device = rapi->getPrimaryDevice();

    mLockedSubresourceIdx = D3D11CalcSubresource(mipLevel, face, mDesc.mipLevels + 1);
    device->getImmediateContext()->Map(res, mLockedSubresourceIdx, flags, 0, &pMappedResource);

    if (device->hasError()) {
        FString errorDescription = device->getErrorDescription();
        EXCEPT(FLogTexture, RenderAPIException, TEXT("D3D11 device cannot map texture\nError Description: %ls"), *errorDescription);
    }

    rowPitch = pMappedResource.RowPitch;
    slicePitch = pMappedResource.DepthPitch;

    return pMappedResource.pData;
}

void FDX11Texture::unmap(ID3D11Resource *res) {
    auto *rapi = static_cast<FDX11RenderAPI *>(FRenderAPI::InstancePtr());
    auto device = rapi->getPrimaryDevice();
    device->getImmediateContext()->Unmap(res, mLockedSubresourceIdx);

    if (device->hasError()) {
        FString errorDescription = device->getErrorDescription();
        EXCEPT(FLogTexture, RenderAPIException, TEXT("D3D11 device unmap resource\nError Description: %ls"), *errorDescription);
    }
}

void *FDX11Texture::mapstagingbuffer(D3D11_MAP flags, uint32_t mipLevel, uint32_t face, uint32_t &rowPitch,
                                     uint32_t &slicePitch) {
    if (!mStagingBuffer) {
        createStagingBuffer();
    }

    auto *rapi = static_cast<FDX11RenderAPI *>(FRenderAPI::InstancePtr());
    auto device = rapi->getPrimaryDevice();
    device->getImmediateContext()->CopyResource(mStagingBuffer, mTexture);

    return map(mStagingBuffer, flags, face, mipLevel, rowPitch, slicePitch);
}

void FDX11Texture::unmapstagingbuffer() {
    unmap(mStagingBuffer);
    SAFE_RELEASE(mStagingBuffer);
}

void *FDX11Texture::mapstaticbuffer(FPixelData lock, uint32_t mipLevel, uint32_t face) {
    uint32_t sizeOfImage = lock.getConsecutiveSize();
    mLockedSubresourceIdx = D3D11CalcSubresource(mipLevel, face, mDesc.mipLevels + 1);

    mStaticBuffer = new FPixelData(lock.getWidth(), lock.getHeight(), lock.getDepth(), lock.getFormat());
    mStaticBuffer->allocateInternalBuffer();

    return mStaticBuffer->getData();
}

void FDX11Texture::unmapstaticbuffer() {
    uint32_t rowWidth = FDX11Mapper::GetSizeInBytes(mStaticBuffer->getFormat(), mStaticBuffer->getWidth());
    uint32_t sliceWidth = FDX11Mapper::GetSizeInBytes(mStaticBuffer->getFormat(), mStaticBuffer->getWidth(), mStaticBuffer->getHeight());

    auto *rapi = static_cast<FDX11RenderAPI *>(FRenderAPI::InstancePtr());
    auto device = rapi->getPrimaryDevice();
    device->getImmediateContext()->UpdateSubresource(mTexture, mLockedSubresourceIdx, nullptr, mStaticBuffer->getData(), rowWidth, sliceWidth);

    if (device->hasError()) {
        FString errorDescription = device->getErrorDescription();
        EXCEPT(FLogTexture, RenderAPIException, TEXT("D3D11 device cannot map texture\nError Description: %ls"), *errorDescription);
    }

    if(mStaticBuffer != nullptr) {
        delete mStaticBuffer;
    }
}
