#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/CommandBuffer.h"
#include "Misc/Module.h"
#include "CommandBufferManager.g.h"

QCLASS(abstract)
class DLL_EXPORT FCommandBufferManager : public TModule<FCommandBufferManager> {
    GENERATED_BODY()

public:
    virtual ~FCommandBufferManager() = default;

    FCommandBuffer *create(EGpuQueueType type, uint32_t deviceIdx = 0, uint32_t queueIdx = 0, bool secondary = false);

private:
    virtual FCommandBuffer *createInternal(EGpuQueueType type, uint32_t deviceIdx, uint32_t queueIdx, bool secondary = false) = 0;
};
