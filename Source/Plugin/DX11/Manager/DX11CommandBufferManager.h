#pragma once

#include "DX11Prerequisites.h"
#include "Manager/CommandBufferManager.h"
#include "DX11CommandBufferManager.g.h"

QCLASS()
class DX11_EXPORT DX11CommandBufferManager : public CommandBufferManager {
    GENERATED_BODY()

private:
    CommandBuffer *createInternal(EGpuQueueType type, uint32_t deviceIdx, uint32_t queueIdx, bool secondary) override;

};
