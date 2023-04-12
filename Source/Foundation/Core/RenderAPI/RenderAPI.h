#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/CommandBuffer.h"
#include "RenderAPI/RenderAPICapabilities.h"
#include "RenderAPI/RenderWindow.h"
#include "Image/Color.h"
#include "Math/Rect.h"
#include "Misc/Module.h"
#include "RenderAPI.g.h"
#include "GpuParamDesc.h"

DLL_EXPORT DECLARE_LOG_CATEGORY_EXTERN(FLogRenderAPI, Debug);

QCLASS(abstract)
class DLL_EXPORT FRenderAPI : public TModule<FRenderAPI> {
    GENERATED_BODY();

public:
    FVideoModeInfo *mVideoModeInfo = nullptr;

    uint32_t mDeviceCount;
    FRenderAPICapabilities *mCurrentCapabilities;

public:
    virtual void setGraphicsPipeline(FGraphicsPipelineState *pipelineState, FCommandBuffer *commandBuffer = nullptr) = 0;
    virtual void setGpuParams(FGpuParams *params, FCommandBuffer *commandBuffer = nullptr) = 0;
    virtual void setVertexDeclaration(FVertexDeclaration *declaration, FCommandBuffer *commandBuffer = nullptr) = 0;
    virtual void setVertexBuffer(uint32_t index, const TArray<FVertexBuffer *> &buffers, FCommandBuffer *commandBuffer = nullptr) = 0;
    virtual void setViewport(const FRect &area, FCommandBuffer *commandBuffer = nullptr) = 0;
    virtual void setIndexBuffer(FIndexBuffer *buffer, FCommandBuffer *commandBuffer = nullptr)  = 0;
    virtual void setRenderTarget(FRenderTarget *target, FCommandBuffer *commandBuffer = nullptr) = 0;

    virtual void clearRenderTarget(EFrameBufferType buffers, const FColor &color = FColor::Green, FCommandBuffer *commandBuffer = nullptr) = 0;
    virtual void swapBuffer(FRenderTarget *target, uint32_t mask = 0xffffffff) = 0;

    virtual void draw(uint32_t vertexOffset, uint32_t vertexCount, uint32_t instanceCount = 0, FCommandBuffer *commandBuffer = nullptr) = 0;
    virtual void drawIndexed(uint32_t indexOffset, uint32_t indexCount, uint32_t vertexOffset, uint32_t vertexCount,
                             uint32_t instanceCount = 0, FCommandBuffer *commandBuffer = nullptr) = 0;
    virtual void submitCommandBuffer(FCommandBuffer *commandBuffer, uint32_t syncMask = 0xFFFFFFFF) = 0;

    virtual const FString &getName() = 0;
    const FRenderAPICapabilities& getCapabilities(uint32_t deviceIdx) const;

    FRenderWindow *initialize(const FRenderWindowDesc &desc);

    virtual FGpuParamBlockDesc generateParamBlockDesc(const FString &name, TArray<FGpuParamDataDesc> &params) = 0;

protected:
    virtual void initialize();
    virtual void initializeWithWindow(FRenderWindow *window);
};

DLL_EXPORT FRenderAPI &gRenderAPI();

inline const FRenderAPICapabilities& gCaps(uint32_t deviceIdx = 0) {
    return FRenderAPI::Instance().getCapabilities(deviceIdx);
}