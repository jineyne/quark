#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/Buffer.h"
#include "RenderAPI/IndexBuffer.h"
#include "RenderAPI/GpuBuffer.h"
#include "RenderAPI/VertexBuffer.h"
#include "Misc/Module.h"
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
