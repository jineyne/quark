#pragma once

#include "CorePrerequisites.h"

enum class ECommandBufferState {
    Empty,
    Recording,
    Executing,
    Done
};

class DLL_EXPORT FCommandBuffer {
protected:
    EGpuQueueType mType;

    uint32_t mDeviceIdx;
    uint32_t mQueueIdx;

    bool mIsSecondary;

public:
    static FCommandBuffer *New(EGpuQueueType type, uint32_t deviceIdx = 0, uint32_t queueIdx = 0, bool secondary = false);

public:
    EGpuQueueType getType() const { return mType; }
    uint32_t getQueueIdx() const { return mQueueIdx; }
    uint32_t getDeviceIdx() const { return mDeviceIdx; }
    virtual ECommandBufferState getState() const = 0;

    virtual void reset() = 0;

protected:
    FCommandBuffer(EGpuQueueType type, uint32_t deviceIdx, uint32_t queueIdx, bool secondary);
};
