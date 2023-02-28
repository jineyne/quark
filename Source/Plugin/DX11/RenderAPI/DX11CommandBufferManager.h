#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/CommandBufferManager.h"
#include "DX11CommandBufferManager.g.h"

QCLASS()
class DX11_EXPORT FDX11CommandBufferManager : public FCommandBufferManager {
    GENERATED_BODY()

private:
    FCommandBuffer *createInternal(EGpuQueueType type, uint32_t deviceIdx, uint32_t queueIdx, bool secondary) override;
};
