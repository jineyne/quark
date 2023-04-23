#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/GpuBuffer.h"

class FDX11GpuBuffer : public FGpuBuffer {
private:
    ID3D11ShaderResourceView* mShadeResourceView = nullptr;
    ID3D11UnorderedAccessView* mUnorderedAccessView = nullptr;

public:
    FDX11GpuBuffer(const FGpuBufferDesc &desc);

public:
    ID3D11Buffer *getBuffer() const;

    ID3D11ShaderResourceView *getShadeResourceView() const;

    ID3D11UnorderedAccessView *getUnorderedAccessView() const;
};
