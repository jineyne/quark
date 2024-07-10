#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/BlendState.h"
#include "RenderAPI/GpuPipelineParamInfo.h"
#include "RenderAPI/SamplerState.h"
#include "RenderAPI/DepthStencilState.h"
#include "RenderAPI/RasterizerState.h"
#include "Misc/Module.h"
#include "RenderStateManager.g.h"

QCLASS(abstract)
class DLL_EXPORT RenderStateManager : public TModule<RenderStateManager> {
    GENERATED_BODY()

private:
    mutable SamplerState *mDefaultSamplerState = nullptr;
    mutable TMap<SamplerStateDesc, SamplerState*> mCachedSamplerStateMap;

    mutable DepthStencilState *mDefaultDepthStencilState = nullptr;
    mutable TMap<DepthStencilStateDesc, DepthStencilState*> mCachedDepthStencilStateMap;

    mutable BlendState *mDefaultBlendState = nullptr;
    mutable TMap<BlendStateDesc, BlendState*> mCachedBlendStateMap;

    mutable RasterizerState *mDefaultRasterizerState = nullptr;
    mutable TMap<RasterizerStateDesc, RasterizerState*> mCachedRasterizerStateMap;

    mutable bool bIsShutdown = false;

public:
    FGpuPipelineParamInfo *createPipelineParamInfo(const FGpuPipelineParamsDesc& desc) const;

    BlendState *createBlendState(const BlendStateDesc &desc) const;
    SamplerState *createSamplerState(const SamplerStateDesc &desc) const;
    DepthStencilState *createDepthStencilState(const DepthStencilStateDesc& desc) const;
    RasterizerState *createRasterizerState(const RasterizerStateDesc& desc) const;

    BlendState *getDefaultBlendState() const;
    SamplerState *getDefaultSamplerState() const;
    DepthStencilState *getDefaultDepthStencilState() const;
    RasterizerState *getDefaultRasterizerState() const;

protected:
    virtual void onShutDown() override;

    virtual FGpuPipelineParamInfo *createPipelineParamInfoInternal(const FGpuPipelineParamsDesc& desc) const;

    virtual BlendState *createBlendStateInternal(const BlendStateDesc &desc) const = 0;
    virtual SamplerState *createSamplerStateInternal(const SamplerStateDesc &desc) const = 0;
    virtual DepthStencilState *createDepthStencilStateInternal(const DepthStencilStateDesc &desc) const = 0;
    virtual RasterizerState *createRasterizerStateInternal(const RasterizerStateDesc &desc) const = 0;

private:
    void notifyBlendStateCreated(const BlendStateDesc &desc, BlendState *state) const;
    void notifyBlendStateDestroyed(const BlendStateDesc &desc) const;

    void notifySamplerStateCreated(const SamplerStateDesc &desc, SamplerState *state) const;
    void notifySamplerStateDestroyed(const SamplerStateDesc &desc) const;

    void notifyDepthStencilStateCreated(const DepthStencilStateDesc &desc, DepthStencilState *state) const;
    void notifyDepthStencilStateDestroyed(const DepthStencilStateDesc &desc) const;

    void notifyRasterizerStateCreated(const RasterizerStateDesc &desc, RasterizerState *state) const;
    void notifyRasterizerStateDestroyed(const RasterizerStateDesc &desc) const;

    BlendState *findCachedState(const BlendStateDesc &desc) const;
    SamplerState *findCachedState(const SamplerStateDesc &desc) const;
    DepthStencilState *findCachedState(const DepthStencilStateDesc &desc) const;
    RasterizerState *findCachedState(const RasterizerStateDesc &desc) const;

private:
    friend class BlendState;
    friend class SamplerState;
    friend class DepthStencilState;
    friend class RasterizerState;
};

DLL_EXPORT RenderStateManager &gRenderStateManager();