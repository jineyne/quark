#pragma once

#include "CorePrerequisites.h"
#include "Misc/Module.h"
#include "RenderAPI/CommandBuffer.h"
#include "RenderAPI/RenderWindow.h"
#include "Image/Color.h"
#include "RenderAPI.g.h"

QCLASS(abstract)
class DLL_EXPORT FRenderAPI : public TModule<FRenderAPI> {
    GENERATED_BODY();

public:
    FVideoModeInfo *mVideoModeInfo = nullptr;
    uint32_t mDeviceId = 0;

public:
    virtual void setRenderTarget(FRenderTarget *target, FCommandBuffer *commandBuffer = nullptr) = 0;

    virtual void clearRenderTarget(EFrameBufferType buffers, const FColor &color = FColor::Green, FCommandBuffer *commandBuffer = nullptr) = 0;
    virtual void swapBuffer(FRenderTarget *target, uint32_t mask = 0xffffffff) = 0;
    virtual void submitCommandBuffer(FCommandBuffer *commandBuffer, uint32_t syncMask = 0xFFFFFFFF) = 0;

    virtual const FString &getName() = 0;

    FRenderWindow *initialize(const FRenderWindowDesc &desc);

protected:
    virtual void initialize();
    virtual void initializeWithWindow(FRenderWindow *window);
};

DLL_EXPORT FRenderAPI &gRenderAPI();