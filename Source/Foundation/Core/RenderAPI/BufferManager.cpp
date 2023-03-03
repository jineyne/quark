#include "BufferManager.h"

FVertexBuffer *FBufferManager::createVertexBuffer(const FVertexBufferDesc &desc) {
    return createVertexBufferInternal(desc);
}

FIndexBuffer *FBufferManager::createIndexBuffer(const FIndexBufferDesc &desc) {
    return createIndexBufferInternal(desc);
}

FGpuBuffer *FBufferManager::createGpuBuffer(const FGpuBufferDesc &desc) {
    return createGpuBufferInternal(desc);
}

