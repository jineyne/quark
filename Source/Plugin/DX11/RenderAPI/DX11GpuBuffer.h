#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/GpuBuffer.h"

class DX11GpuBuffer : public GpuBuffer {
private:
    ID3D11ShaderResourceView* mShadeResourceView = nullptr;
    ID3D11UnorderedAccessView* mUnorderedAccessView = nullptr;

public:
    DX11GpuBuffer(const GpuBufferDesc &desc);

public:
    ID3D11Buffer *getBuffer() const;

    ID3D11ShaderResourceView *getShadeResourceView() const;

    ID3D11UnorderedAccessView *getUnorderedAccessView() const;
};
