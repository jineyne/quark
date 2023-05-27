#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/GpuPipelineParamInfo.h"
#include "RenderAPI/SamplerState.h"
#include "Misc/Module.h"
#include "RenderAPI/DepthStencilState.h"
#include "RenderStateManager.g.h"

QCLASS(abstract)
class DLL_EXPORT RenderStateManager : public TModule<RenderStateManager> {
    GENERATED_BODY()

private:
    mutable SamplerState *mDefaultSamplerState = nullptr;
    mutable TMap<SamplerStateDesc, SamplerState*> mCachedSamplerStateMap;

    mutable DepthStencilState *mDefaultDepthStencilState = nullptr;
    mutable TMap<DepthStencilStateDesc, DepthStencilState*> mCachedDepthStencilStateMap;

    mutable bool bIsShutdown = false;

public:
    FGpuPipelineParamInfo *createPipelineParamInfo(const FGpuPipelineParamsDesc& desc) const;
    SamplerState *createSamplerState(const SamplerStateDesc &desc) const;
    DepthStencilState *createDepthStencilState(const DepthStencilStateDesc& desc) const;

    SamplerState *getDefaultSamplerState() const;
    DepthStencilState *getDefaultDepthStencilState() const;

protected:
    virtual void onShutDown() override;

    virtual FGpuPipelineParamInfo *createPipelineParamInfoInternal(const FGpuPipelineParamsDesc& desc) const;
    virtual SamplerState *createSamplerStateInternal(const SamplerStateDesc &desc) const = 0;
    virtual DepthStencilState *createDepthStencilStateInternal(const DepthStencilStateDesc &desc) const = 0;

private:
    void notifySamplerStateCreated(const SamplerStateDesc &desc, SamplerState *state) const;
    void notifySamplerStateDestroyed(const SamplerStateDesc &desc) const;

    void notifyDepthStencilStateCreated(const DepthStencilStateDesc &desc, DepthStencilState *state) const;
    void notifyDepthStencilStateDestroyed(const DepthStencilStateDesc &desc) const;

    SamplerState *findCachedState(const SamplerStateDesc &desc) const;
    DepthStencilState *findCachedState(const DepthStencilStateDesc &desc) const;

private:
    friend class SamplerState;
    friend class DepthStencilState;
};

DLL_EXPORT RenderStateManager &gRenderStateManager();