#include "DX11CommandBuffer.h"

#include "Exception/Exception.h"

#include "DX11EventQuery.h"

DX11CommandBuffer::DX11CommandBuffer(EGpuQueueType type, UINT32 deviceIdx, UINT32 queueIdx, bool secondary)
        : CommandBuffer(type, deviceIdx, queueIdx, secondary) {
    if (deviceIdx != 0) {
        EXCEPT(LogDX11, InvalidParametersException, TEXT("Only a single device supported on DX11."));
    }
}

DX11CommandBuffer::~DX11CommandBuffer() {
    if (mFence != nullptr) {
        delete mFence;
    }
}

void DX11CommandBuffer::queueCommand(const std::function<void()> command) {
#if DEBUG_MODE
    if (getState() == ECommandBufferState::Executing)
    {
        LOG(LogDX11, Error, TEXT("Cannot modify a command buffer that's still executing."));
        return;
    }
#endif

    command();
    mCommandQueued = true;
}

void DX11CommandBuffer::executeCommands() {
#if DEBUG_MODE
    if (mIsSecondary) {
        LOG(LogDX11, Error, TEXT("Cannot execute commands on a secondary buffer."));
        return;
    }

    if(getState() == ECommandBufferState::Executing) {
        LOG(LogDX11, Error, TEXT("Cannot submit a command buffer that's still executing."));
        return;
    }
#endif

    if (mFence != nullptr) {
        delete mFence;
    }

    mFence = q_new<DX11EventQuery>(mDeviceIdx);
    mFence->begin();
    mIsSubmitted = true;
}

ECommandBufferState DX11CommandBuffer::getState() const {
    if (mIsSubmitted) {
        return isComplete() ? ECommandBufferState::Done : ECommandBufferState::Executing;
    }

    return mCommandQueued ? ECommandBufferState::Recording : ECommandBufferState::Empty;
}

void DX11CommandBuffer::reset() {
    mCommandQueued = false;
    mIsSubmitted = false;
}

bool DX11CommandBuffer::isComplete() const {
    return mFence && mFence->isReady();
}
