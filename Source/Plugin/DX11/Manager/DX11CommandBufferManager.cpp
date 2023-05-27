#include "DX11CommandBufferManager.h"

#include "RenderAPI/DX11CommandBuffer.h"

CommandBuffer *DX11CommandBufferManager::createInternal(EGpuQueueType type, uint32_t deviceIdx, uint32_t queueIdx, bool secondary) {
    auto data = q_alloc<DX11CommandBuffer>();
    return new (data) DX11CommandBuffer(type, deviceIdx, queueIdx, secondary);
}

