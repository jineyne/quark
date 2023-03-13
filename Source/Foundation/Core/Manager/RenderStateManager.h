#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/GpuPipelineParamInfo.h"
#include "RenderAPI/SamplerState.h"
#include "Misc/Module.h"
#include "RenderStateManager.g.h"

QCLASS()
class DLL_EXPORT FRenderStateManager : public TModule<FRenderStateManager> {
    GENERATED_BODY()

private:
    mutable FSamplerState *mDefaultSamplerState = nullptr;
    mutable TMap<FSamplerStateDesc, FSamplerState*> mCachedSamplerStateMap;

public:
    FGpuPipelineParamInfo *createPipelineParamInfo(const FGpuPipelineParamsDesc& desc) const;
    FSamplerState *createSamplerState(const FSamplerStateDesc &desc) const;

    FSamplerState *getDefaultSamplerState() const;

protected:
    virtual void onShutDown() override;

    virtual FGpuPipelineParamInfo *createPipelineParamInfoInternal(const FGpuPipelineParamsDesc& desc) const;
    virtual FSamplerState *createSamplerStateInternal(const FSamplerStateDesc &desc) const;

private:
    void notifySamplerStateCreated(const FSamplerStateDesc &desc, FSamplerState *state) const;
    void notifySamplerStateDestroyed(const FSamplerStateDesc &desc) const;

    FSamplerState *findCachedState(const FSamplerStateDesc &desc) const;

private:
    friend class SamplerState;
};

DLL_EXPORT FRenderStateManager &gRenderStateManager();