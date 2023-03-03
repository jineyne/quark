#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/GpuBuffer.h"

class FDX11GpuBuffer : public FGpuBuffer {
public:
    FDX11GpuBuffer(const FGpuBufferDesc &desc);

public:
    ID3D11Buffer *getBuffer() const;
};
