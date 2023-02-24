#include "CommandBuffer.h"

#include "CommandBufferManager.h"

FCommandBuffer *FCommandBuffer::New(EGpuQueueType type, uint32_t deviceIdx, uint32_t queueIdx, bool secondary) {
    return FCommandBufferManager::Instance().create(type, deviceIdx, queueIdx, secondary);
}

FCommandBuffer::FCommandBuffer(EGpuQueueType type, uint32_t deviceIdx, uint32_t queueIdx, bool secondary)
        : mType(type), mDeviceIdx(deviceIdx), mQueueIdx(queueIdx), mIsSecondary(secondary) { }
