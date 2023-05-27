#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/CommandBuffer.h"

class DX11_EXPORT DX11CommandBuffer : public CommandBuffer  {
private:
    DX11EventQuery *mFence = nullptr;
    bool mCommandQueued = false;
    bool mIsSubmitted = false;

protected:
    DX11CommandBuffer(EGpuQueueType type, UINT32 deviceIdx, UINT32 queueIdx, bool secondary);

public:
    ~DX11CommandBuffer();

public:
    void queueCommand(const std::function<void()> command);
    void executeCommands();

    ECommandBufferState getState() const override;
    void reset() override;

private:
    friend class DX11CommandBufferManager;

    bool isComplete() const;
};
