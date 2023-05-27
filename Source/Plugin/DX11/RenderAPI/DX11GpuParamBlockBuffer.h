#pragma once

#include "DX11Prerequisites.h"

#include "RenderAPI/GpuParamBlockBuffer.h"
#include "RenderAPI/Buffer.h"

class PLUGIN_EXPORT DX11GpuParamBlockBuffer : public GpuParamBlockBuffer {
public:
    DX11GpuParamBlockBuffer(uint32_t size, EBufferUsage usage);
    ~DX11GpuParamBlockBuffer() override;

public:
    ID3D11Buffer* getBuffer() const;
};