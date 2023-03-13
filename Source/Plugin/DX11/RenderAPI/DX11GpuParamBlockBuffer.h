#pragma once

#include "DX11Prerequisites.h"

#include "RenderAPI/GpuParamBlockBuffer.h"
#include "RenderAPI/Buffer.h"

class PLUGIN_EXPORT FDX11GpuParamBlockBuffer : public FGpuParamBlockBuffer {
public:
    FDX11GpuParamBlockBuffer(uint32_t size, EBufferUsage usage);
    ~FDX11GpuParamBlockBuffer() override;

public:
    ID3D11Buffer* getBuffer() const;
};