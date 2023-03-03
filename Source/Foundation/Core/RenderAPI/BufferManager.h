#pragma once

#include "CorePrerequisites.h"
#include "Misc/Module.h"
#include "Buffer.h"
#include "IndexBuffer.h"
#include "GpuBuffer.h"
#include "VertexBuffer.h"
#include "BufferManager.g.h"

QCLASS(abstract)
class DLL_EXPORT FBufferManager : public TModule<FBufferManager> {
    GENERATED_BODY()

public:
    FVertexBuffer *createVertexBuffer(const FVertexBufferDesc &desc);
    FIndexBuffer *createIndexBuffer(const FIndexBufferDesc &desc);

    FGpuBuffer *createGpuBuffer(const FGpuBufferDesc &desc);

protected:
    virtual FVertexBuffer *createVertexBufferInternal(const FVertexBufferDesc &desc) = 0;
    virtual FIndexBuffer *createIndexBufferInternal(const FIndexBufferDesc &desc) = 0;
    virtual FGpuBuffer *createGpuBufferInternal(const FGpuBufferDesc &desc) = 0;
};
