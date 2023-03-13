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
class DLL_EXPORT FBufferManager : public TModule<FBufferManager> {
    GENERATED_BODY()

public:
    FVertexDeclaration *createVertexDeclaration(FVertexDataDesc *desc);
    FVertexDeclaration *createVertexDeclaration(TArray<FVertexElement> &elementList);

    FVertexBuffer *createVertexBuffer(const FVertexBufferDesc &desc);
    FIndexBuffer *createIndexBuffer(const FIndexBufferDesc &desc);

    FGpuParamBlockBuffer *createGpuParamBlockBuffer(uint32_t size, EBufferUsage usage);
    FGpuBuffer *createGpuBuffer(const FGpuBufferDesc &desc);
    FGpuParams *createGpuParams(FGpuPipelineParamInfo *paramInfo);

protected:
    virtual FVertexBuffer *createVertexBufferInternal(const FVertexBufferDesc &desc) = 0;
    virtual FIndexBuffer *createIndexBufferInternal(const FIndexBufferDesc &desc) = 0;

    virtual FGpuParamBlockBuffer *createGpuParamBlockBufferInternal(uint32_t size, EBufferUsage usage) = 0;
    virtual FGpuBuffer *createGpuBufferInternal(const FGpuBufferDesc &desc) = 0;
};
