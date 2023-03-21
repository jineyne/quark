#include "DX11CommandBufferManager.h"

#include "RenderAPI/DX11CommandBuffer.h"

FCommandBuffer *FDX11CommandBufferManager::createInternal(EGpuQueueType type, uint32_t deviceIdx, uint32_t queueIdx, bool secondary) {
    auto data = q_alloc<FDX11CommandBuffer>();
    return new (data) FDX11CommandBuffer(type, deviceIdx, queueIdx, secondary);
}

