#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/Buffer.h"
#include "RenderAPI/IndexBuffer.h"
#include "RenderAPI/GpuBuffer.h"
#include "RenderAPI/GpuPipelineParamInfo.h"
#include "RenderAPI/VertexBuffer.h"
#include "RenderAPI/VertexData.h"
#include "RenderAPI/VertexDataDesc.h"
#include "RenderAPI/VertexDeclaration.h"
#include "Misc/Module.h"
#include "BufferManager.g.h"

QCLASS(abstract)
class DLL_EXPORT BufferManager : public TModule<BufferManager> {
    GENERATED_BODY()

public:
    VertexDeclaration *createVertexDeclaration(VertexDataDesc *desc);
    VertexDeclaration *createVertexDeclaration(TArray<VertexElement> &elementList);

    VertexBuffer *createVertexBuffer(const VertexBufferDesc &desc);
    IndexBuffer *createIndexBuffer(const FIndexBufferDesc &desc);

    GpuParamBlockBuffer *createGpuParamBlockBuffer(uint32_t size, EBufferUsage usage);
    GpuBuffer *createGpuBuffer(const GpuBufferDesc &desc);
    GpuParams *createGpuParams(FGpuPipelineParamInfo *paramInfo);

protected:
    virtual VertexBuffer *createVertexBufferInternal(const VertexBufferDesc &desc) = 0;
    virtual IndexBuffer *createIndexBufferInternal(const FIndexBufferDesc &desc) = 0;

    virtual GpuParamBlockBuffer *createGpuParamBlockBufferInternal(uint32_t size, EBufferUsage usage) = 0;
    virtual GpuBuffer *createGpuBufferInternal(const GpuBufferDesc &desc) = 0;
};
