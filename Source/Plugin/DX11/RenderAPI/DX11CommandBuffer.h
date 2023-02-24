#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/CommandBuffer.h"

class DLL_EXPORT FDX11CommandBuffer : public FCommandBuffer  {
private:
    FDX11EventQuery *mFence;
    bool mCommandQueued = false;
    bool mIsSubmitted = false;

protected:
    FDX11CommandBuffer(EGpuQueueType type, UINT32 deviceIdx, UINT32 queueIdx, bool secondary);

public:
    void queueCommand(const std::function<void()> command);
    void executeCommands();

    ECommandBufferState getState() const override;
    void reset() override;

private:
    friend class FDX11CommandBufferManager;

    bool isComplete() const;
};
