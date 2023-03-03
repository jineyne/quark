#pragma once

#include "DX11Prerequisites.h"
#include "Manager/BufferManager.h"
#include "DX11BufferManager.g.h"

QCLASS()
class DX11_EXPORT FDX11BufferManager : public FBufferManager {
    GENERATED_BODY()

protected:
    FVertexBuffer *createVertexBufferInternal(const FVertexBufferDesc &desc) override;
    FIndexBuffer *createIndexBufferInternal(const FIndexBufferDesc &desc) override;

    FGpuBuffer *createGpuBufferInternal(const FGpuBufferDesc &desc) override;
};