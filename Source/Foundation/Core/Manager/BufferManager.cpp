#include "BufferManager.h"
#include "RenderAPI/GpuParams.h"

VertexDeclaration *BufferManager::createVertexDeclaration(VertexDataDesc *desc) {
    return q_new<VertexDeclaration>(desc->createElements());
}

VertexDeclaration *BufferManager::createVertexDeclaration(TArray<VertexElement> &elementList) {
    return q_new<VertexDeclaration>(elementList);
}

VertexBuffer *BufferManager::createVertexBuffer(const VertexBufferDesc &desc) {
    return createVertexBufferInternal(desc);
}

IndexBuffer *BufferManager::createIndexBuffer(const FIndexBufferDesc &desc) {
    return createIndexBufferInternal(desc);
}

GpuParamBlockBuffer *BufferManager::createGpuParamBlockBuffer(uint32_t size, EBufferUsage usage) {
    return createGpuParamBlockBufferInternal(size, usage);
}

GpuBuffer *BufferManager::createGpuBuffer(const GpuBufferDesc &desc) {
    return createGpuBufferInternal(desc);
}

GpuParams *BufferManager::createGpuParams(FGpuPipelineParamInfo *paramInfo) {
    return q_new<GpuParams>(paramInfo);
}


