#include "DX11CommandBuffer.h"

#include "Exception/Exception.h"

#include "DX11EventQuery.h"

FDX11CommandBuffer::FDX11CommandBuffer(EGpuQueueType type, UINT32 deviceIdx, UINT32 queueIdx, bool secondary)
        : FCommandBuffer(type, deviceIdx, queueIdx, secondary) {
    if (deviceIdx != 0) {
        EXCEPT(FLogDX11, InvalidParametersException, TEXT("Only a single device supported on DX11."));
    }
}

void FDX11CommandBuffer::queueCommand(const std::function<void()> command) {
#if DEBUG_MODE
    if (getState() == ECommandBufferState::Executing)
    {
        LOG(FLogDX11, Error, TEXT("Cannot modify a command buffer that's still executing."));
        return;
    }
#endif

    command();
    mCommandQueued = true;
}

void FDX11CommandBuffer::executeCommands() {
#if DEBUG_MODE
    if (mIsSecondary) {
        LOG(FLogDX11, Error, TEXT("Cannot execute commands on a secondary buffer."));
        return;
    }

    if(getState() == ECommandBufferState::Executing) {
        LOG(FLogDX11, Error, TEXT("Cannot submit a command buffer that's still executing."));
        return;
    }
#endif

    mFence = new FDX11EventQuery(mDeviceIdx);
    mFence->begin();
    mIsSubmitted = true;
}

ECommandBufferState FDX11CommandBuffer::getState() const {
    if (mIsSubmitted) {
        return isComplete() ? ECommandBufferState::Done : ECommandBufferState::Executing;
    }

    return mCommandQueued ? ECommandBufferState::Recording : ECommandBufferState::Empty;
}

void FDX11CommandBuffer::reset() {
    mCommandQueued = false;
    mIsSubmitted = false;
}

bool FDX11CommandBuffer::isComplete() const {
    return mFence && mFence->isReady();
}
