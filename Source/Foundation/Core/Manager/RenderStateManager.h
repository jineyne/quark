#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/GpuPipelineParamInfo.h"
#include "RenderAPI/SamplerState.h"
#include "Misc/Module.h"
#include "RenderAPI/DepthStencilState.h"
#include "RenderStateManager.g.h"

QCLASS(abstract)
class DLL_EXPORT FRenderStateManager : public TModule<FRenderStateManager> {
    GENERATED_BODY()

private:
    mutable FSamplerState *mDefaultSamplerState = nullptr;
    mutable TMap<FSamplerStateDesc, FSamplerState*> mCachedSamplerStateMap;

    mutable FDepthStencilState *mDefaultDepthStencilState = nullptr;
    mutable TMap<FDepthStencilStateDesc, FDepthStencilState*> mCachedDepthStencilStateMap;

public:
    FGpuPipelineParamInfo *createPipelineParamInfo(const FGpuPipelineParamsDesc& desc) const;
    FSamplerState *createSamplerState(const FSamplerStateDesc &desc) const;
    FDepthStencilState *createDepthStencilState(const FDepthStencilStateDesc& desc) const;

    FSamplerState *getDefaultSamplerState() const;
    FDepthStencilState *getDefaultDepthStencilState() const;

protected:
    virtual void onShutDown() override;

    virtual FGpuPipelineParamInfo *createPipelineParamInfoInternal(const FGpuPipelineParamsDesc& desc) const;
    virtual FSamplerState *createSamplerStateInternal(const FSamplerStateDesc &desc) const = 0;
    virtual FDepthStencilState *createDepthStencilStateInternal(const FDepthStencilStateDesc &desc) const = 0;

private:
    void notifySamplerStateCreated(const FSamplerStateDesc &desc, FSamplerState *state) const;
    void notifySamplerStateDestroyed(const FSamplerStateDesc &desc) const;

    void notifyDepthStencilStateCreated(const FDepthStencilStateDesc &desc, FDepthStencilState *state) const;
    void notifyDepthStencilStateDestroyed(const FDepthStencilStateDesc &desc) const;

    FSamplerState *findCachedState(const FSamplerStateDesc &desc) const;
    FDepthStencilState *findCachedState(const FDepthStencilStateDesc &desc) const;

private:
    friend class FSamplerState;
    friend class FDepthStencilState;
};

DLL_EXPORT FRenderStateManager &gRenderStateManager();