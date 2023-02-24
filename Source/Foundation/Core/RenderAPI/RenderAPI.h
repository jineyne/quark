#pragma once

#include "CorePrerequisites.h"
#include "Misc/Module.h"
#include "CommandBuffer.h"
#include "RenderWindow.h"
#include "RenderAPI.g.h"

QCLASS(abstract)
class DLL_EXPORT FRenderAPI : public TModule<FRenderAPI> {
    GENERATED_BODY();

public:
    FVideoModeInfo *mVideoModeInfo = nullptr;
    uint32_t mDeviceId = 0;

public:
    virtual void setRenderTarget(FRenderTarget *target, FCommandBuffer *commandBuffer = nullptr) = 0;

    virtual const FString &getName() = 0;

    FRenderWindow *initialize(const FRenderWindowDesc &desc);

protected:
    virtual void initialize();
    virtual void initializeWithWindow(FRenderWindow *window);
};

DLL_EXPORT FRenderAPI &gRenderAPI();