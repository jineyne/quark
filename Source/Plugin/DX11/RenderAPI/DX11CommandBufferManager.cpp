#include "DX11CommandBufferManager.h"

#include "DX11CommandBuffer.h"

FCommandBuffer *FDX11CommandBufferManager::createInternal(EGpuQueueType type, uint32_t deviceIdx, uint32_t queueIdx, bool secondary) {
    return new FDX11CommandBuffer(type, deviceIdx, queueIdx, secondary);
}

