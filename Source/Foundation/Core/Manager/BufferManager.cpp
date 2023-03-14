#include "BufferManager.h"
#include "RenderAPI/GpuParams.h"

FVertexDeclaration *FBufferManager::createVertexDeclaration(FVertexDataDesc *desc) {
    return new FVertexDeclaration(desc->createElements());
}

FVertexDeclaration *FBufferManager::createVertexDeclaration(TArray<FVertexElement> &elementList) {
    return new FVertexDeclaration(elementList);
}

FVertexBuffer *FBufferManager::createVertexBuffer(const FVertexBufferDesc &desc) {
    return createVertexBufferInternal(desc);
}

FIndexBuffer *FBufferManager::createIndexBuffer(const FIndexBufferDesc &desc) {
    return createIndexBufferInternal(desc);
}

FGpuParamBlockBuffer *FBufferManager::createGpuParamBlockBuffer(uint32_t size, EBufferUsage usage) {
    return createGpuParamBlockBufferInternal(size, usage);
}

FGpuBuffer *FBufferManager::createGpuBuffer(const FGpuBufferDesc &desc) {
    return createGpuBufferInternal(desc);
}

FGpuParams *FBufferManager::createGpuParams(FGpuPipelineParamInfo *paramInfo) {
    return new FGpuParams(paramInfo);
}

