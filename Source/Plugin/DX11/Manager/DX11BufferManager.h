#pragma once

#include "DX11Prerequisites.h"
#include "Manager/BufferManager.h"
#include "DX11BufferManager.g.h"

QCLASS()
class DX11_EXPORT DX11BufferManager : public BufferManager {
    GENERATED_BODY()

protected:
    VertexBuffer *createVertexBufferInternal(const VertexBufferDesc &desc) override;
    IndexBuffer *createIndexBufferInternal(const FIndexBufferDesc &desc) override;

    GpuParamBlockBuffer *createGpuParamBlockBufferInternal(uint32_t size, EBufferUsage usage);
    GpuBuffer *createGpuBufferInternal(const GpuBufferDesc &desc) override;

};