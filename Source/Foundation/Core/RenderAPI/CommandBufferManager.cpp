#include "CommandBufferManager.h"

FCommandBuffer *FCommandBufferManager::create(EGpuQueueType type, uint32_t deviceIdx, uint32_t queueIdx, bool secondary) {
    return createInternal(type, deviceIdx, queueIdx, secondary);
}
