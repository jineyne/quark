#include "CommandBuffer.h"

#include "Manager/CommandBufferManager.h"

CommandBuffer *CommandBuffer::New(EGpuQueueType type, uint32_t deviceIdx, uint32_t queueIdx, bool secondary) {
    return CommandBufferManager::Instance().create(type, deviceIdx, queueIdx, secondary);
}

CommandBuffer::CommandBuffer(EGpuQueueType type, uint32_t deviceIdx, uint32_t queueIdx, bool secondary)
        : mType(type), mDeviceIdx(deviceIdx), mQueueIdx(queueIdx), mIsSecondary(secondary) { }
