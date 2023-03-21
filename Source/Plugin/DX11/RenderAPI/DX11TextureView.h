#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/TextureView.h"
#include "Image/DX11Texture.h"

class DX11_EXPORT FDX11TextureView : public FTextureView {
private:
    ID3D11ShaderResourceView* mSRV = nullptr;
    ID3D11RenderTargetView* mRTV = nullptr;
    ID3D11UnorderedAccessView* mUAV = nullptr;
    ID3D11DepthStencilView*	mWDepthWStencilView = nullptr;
    ID3D11DepthStencilView*	mRODepthWStencilView = nullptr;
    ID3D11DepthStencilView*	mRODepthROStencilView = nullptr;
    ID3D11DepthStencilView*	mWDepthROStencilView = nullptr;

private:
    FDX11TextureView(const FDX11Texture *texture, const FTextureViewDesc& desc);

public:
    ~FDX11TextureView();

public:
    ID3D11DepthStencilView*	getDSV(bool readOnlyDepth, bool readOnlyStencil) const;

    ID3D11ShaderResourceView* getSRV() const { return mSRV; }
    ID3D11RenderTargetView*	getRTV() const { return mRTV; }
    ID3D11UnorderedAccessView* getUAV() const { return mUAV; }

private:
    ID3D11ShaderResourceView* createSRV(const FDX11Texture* texture, uint32_t mostDetailMip, uint32_t numMips,
                                        uint32_t firstArraySlice, uint32_t numArraySlices);

    ID3D11RenderTargetView* createRTV(const FDX11Texture* texture, uint32_t mipSlice, uint32_t firstArraySlice,
                                      uint32_t numArraySlices);

    ID3D11UnorderedAccessView* createUAV(const FDX11Texture* texture, uint32_t mipSlice, uint32_t firstArraySlice,
                                         uint32_t numArraySlices);

    ID3D11DepthStencilView* createDSV(const FDX11Texture* texture, uint32_t mipSlice, uint32_t firstArraySlice,
                                      uint32_t numArraySlices, bool readOnlyDepth, bool readOnlyStencil);
};
