#include "DX11TextureView.h"
#include "Exception/Exception.h"

DX11TextureView::DX11TextureView(const DX11Texture *texture, const TextureViewDesc &desc)
        : TextureView(desc) {
    if ((mDesc.usage & EGpuViewUsage::RandomWrite) == EGpuViewUsage::RandomWrite)
        mUAV = createUAV(texture, mDesc.mostDetailMip, mDesc.firstArraySlice, mDesc.arraySlicesCount);
    else if ((mDesc.usage & EGpuViewUsage::RenderTarget) == EGpuViewUsage::RenderTarget)
        mRTV = createRTV(texture, mDesc.mostDetailMip, mDesc.firstArraySlice, mDesc.arraySlicesCount);
    else if ((mDesc.usage & EGpuViewUsage::DepthStencil) == EGpuViewUsage::DepthStencil) {
        mWDepthWStencilView = createDSV(texture, mDesc.mostDetailMip, mDesc.firstArraySlice, mDesc.arraySlicesCount, false, false);
        mRODepthWStencilView = createDSV(texture, mDesc.mostDetailMip, mDesc.firstArraySlice, mDesc.arraySlicesCount, true, false);
        mRODepthROStencilView = createDSV(texture, mDesc.mostDetailMip, mDesc.firstArraySlice, mDesc.arraySlicesCount, true, true);
        mWDepthROStencilView = createDSV(texture, mDesc.mostDetailMip, mDesc.firstArraySlice, mDesc.arraySlicesCount, false, true);
    } else {
        mSRV = createSRV(texture, mDesc.mostDetailMip, mDesc.mipCount, mDesc.firstArraySlice, mDesc.arraySlicesCount);
    }
}

DX11TextureView::~DX11TextureView() {
    SAFE_RELEASE(mSRV);
    SAFE_RELEASE(mUAV);
    SAFE_RELEASE(mRTV);
    SAFE_RELEASE(mWDepthWStencilView);
    SAFE_RELEASE(mWDepthROStencilView);
    SAFE_RELEASE(mRODepthWStencilView);
    SAFE_RELEASE(mRODepthROStencilView);
}

ID3D11DepthStencilView *DX11TextureView::getDSV(bool readOnlyDepth, bool readOnlyStencil) const {
    if(readOnlyDepth) {
        if (readOnlyStencil) {
            return mRODepthROStencilView;
        } else {
            return mRODepthWStencilView;
        }
    } else {
        if (readOnlyStencil) {
            return mWDepthROStencilView;
        } else {
            return mWDepthWStencilView;
        }
    }
}

ID3D11ShaderResourceView *DX11TextureView::createSRV(const DX11Texture *texture, uint32_t mostDetailMip,
                                                     uint32_t numMips, uint32_t firstArraySlice, uint32_t numArraySlices) {
    /*D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    uint32_t numFaces = texture->getFaceCount();

    switch (texture->getTextureType())
    {
        case Type2D:
            if (numFaces <= 1)
            {
                desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
                desc.Texture1D.MipLevels = numMips;
                desc.Texture1D.MostDetailedMip = mostDetailMip;
            }
            else
            {
                desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
                desc.Texture1DArray.MipLevels = numMips;
                desc.Texture1DArray.MostDetailedMip = mostDetailMip;
                desc.Texture1DArray.FirstArraySlice = firstArraySlice;
                desc.Texture1DArray.ArraySize = numArraySlices;
            }
            break;
        case TEX_TYPE_2D:
            if (texProps.getNumSamples() > 1)
            {
                if (numFaces <= 1)
                {
                    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
                }
                else
                {
                    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
                    desc.Texture2DMSArray.FirstArraySlice = firstArraySlice;
                    desc.Texture2DMSArray.ArraySize = numArraySlices;
                }
            }
            else
            {
                if (numFaces <= 1)
                {
                    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                    desc.Texture2D.MipLevels = numMips;
                    desc.Texture2D.MostDetailedMip = mostDetailMip;
                }
                else
                {
                    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                    desc.Texture2DArray.MipLevels = numMips;
                    desc.Texture2DArray.MostDetailedMip = mostDetailMip;
                    desc.Texture2DArray.FirstArraySlice = firstArraySlice;
                    desc.Texture2DArray.ArraySize = numArraySlices;
                }
            }
            break;
        case TEX_TYPE_3D:
            desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
            desc.Texture3D.MipLevels = numMips;
            desc.Texture3D.MostDetailedMip = mostDetailMip;
            break;
        case TEX_TYPE_CUBE_MAP:
            if(numFaces % 6 == 0)
            {
                if (numFaces == 6)
                {
                    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
                    desc.TextureCube.MipLevels = numMips;
                    desc.TextureCube.MostDetailedMip = mostDetailMip;
                }
                else
                {
                    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
                    desc.TextureCubeArray.MipLevels = numMips;
                    desc.TextureCubeArray.MostDetailedMip = mostDetailMip;
                    desc.TextureCubeArray.First2DArrayFace = firstArraySlice;
                    desc.TextureCubeArray.NumCubes = numArraySlices / 6;
                }
            }
            else
            {
                desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                desc.Texture2DArray.MipLevels = numMips;
                desc.Texture2DArray.MostDetailedMip = mostDetailMip;
                desc.Texture2DArray.FirstArraySlice = firstArraySlice;
                desc.Texture2DArray.ArraySize = numArraySlices;
            }

            break;
        default:
            EXCEPT(FLogTexture, InvalidParametersException, "Invalid texture type for this view type.");
    }

    desc.Format = texture->getColorFormat();

    ID3D11ShaderResourceView* srv = nullptr;

    FD3D11RenderAPI* d3d11rs = static_cast<DX11RenderAPI*>(DX11RenderAPI::InstancePtr());
    HRESULT hr = d3d11rs->getPrimaryDevice().getD3D11Device()->CreateShaderResourceView(texture->getDX11Resource(), &desc, &srv);

    if (FAILED(hr) || d3d11rs->getPrimaryDevice().hasError())
    {
        String msg = d3d11rs->getPrimaryDevice().getErrorDescription();
        BS_EXCEPT(RenderingAPIException, "Cannot create ShaderResourceView: " + msg);
    }

    return srv;*/
    return nullptr;
}

ID3D11RenderTargetView *DX11TextureView::createRTV(const DX11Texture *texture, uint32_t mipSlice,
                                                   uint32_t firstArraySlice, uint32_t numArraySlices) {
    return nullptr;
}

ID3D11UnorderedAccessView *DX11TextureView::createUAV(const DX11Texture *texture, uint32_t mipSlice,
                                                      uint32_t firstArraySlice, uint32_t numArraySlices) {
    return nullptr;
}

ID3D11DepthStencilView *
DX11TextureView::createDSV(const DX11Texture *texture, uint32_t mipSlice, uint32_t firstArraySlice, uint32_t numArraySlices,
                           bool readOnlyDepth, bool readOnlyStencil) {
    return nullptr;
}
