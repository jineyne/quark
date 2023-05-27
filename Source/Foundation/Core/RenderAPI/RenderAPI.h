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

DLL_EXPORT DECLARE_LOG_CATEGORY_EXTERN(LogRenderAPI, Debug);

QCLASS(abstract)
class DLL_EXPORT RenderAPI : public TModule<RenderAPI> {
    GENERATED_BODY();

public:
    VideoModeInfo *mVideoModeInfo = nullptr;

    uint32_t mDeviceCount;
    RenderAPICapabilities *mCurrentCapabilities;

public:
    virtual void setGraphicsPipeline(GraphicsPipelineState *pipelineState, CommandBuffer *commandBuffer = nullptr) = 0;
    virtual void setGpuParams(GpuParams *params, CommandBuffer *commandBuffer = nullptr) = 0;
    virtual void setVertexDeclaration(VertexDeclaration *declaration, CommandBuffer *commandBuffer = nullptr) = 0;
    virtual void setVertexBuffer(uint32_t index, const TArray<VertexBuffer *> &buffers, CommandBuffer *commandBuffer = nullptr) = 0;
    virtual void setViewport(const Rect &area, CommandBuffer *commandBuffer = nullptr) = 0;
    virtual void setIndexBuffer(IndexBuffer *buffer, CommandBuffer *commandBuffer = nullptr)  = 0;
    virtual void setRenderTarget(RenderTarget *target, CommandBuffer *commandBuffer = nullptr) = 0;

    virtual void clearRenderTarget(EFrameBufferType buffers, const Color &color = Color::Green, CommandBuffer *commandBuffer = nullptr) = 0;
    virtual void swapBuffer(RenderTarget *target, uint32_t mask = 0xffffffff) = 0;

    virtual void draw(uint32_t vertexOffset, uint32_t vertexCount, uint32_t instanceCount = 0, CommandBuffer *commandBuffer = nullptr) = 0;
    virtual void drawIndexed(uint32_t indexOffset, uint32_t indexCount, uint32_t vertexOffset, uint32_t vertexCount,
                             uint32_t instanceCount = 0, CommandBuffer *commandBuffer = nullptr) = 0;
    virtual void submitCommandBuffer(CommandBuffer *commandBuffer, uint32_t syncMask = 0xFFFFFFFF) = 0;

    virtual const String &getName() = 0;
    const RenderAPICapabilities& getCapabilities(uint32_t deviceIdx) const;

    RenderWindow *initialize(const RenderWindowDesc &desc);

    virtual GpuParamBlockDesc generateParamBlockDesc(const String &name, TArray<GpuParamDataDesc> &params) = 0;

protected:
    virtual void initialize();
    virtual void initializeWithWindow(RenderWindow *window);
};

DLL_EXPORT RenderAPI &gRenderAPI();

inline const RenderAPICapabilities& gCaps(uint32_t deviceIdx = 0) {
    return RenderAPI::Instance().getCapabilities(deviceIdx);
}