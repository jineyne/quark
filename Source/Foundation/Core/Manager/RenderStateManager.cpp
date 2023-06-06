#include "RenderStateManager.h"

FGpuPipelineParamInfo *RenderStateManager::createPipelineParamInfo(const FGpuPipelineParamsDesc &desc) const {
    return createPipelineParamInfoInternal(desc);
}

SamplerState *RenderStateManager::createSamplerState(const SamplerStateDesc &desc) const {
    auto state = findCachedState(desc);
    if (state == nullptr) {
        state = createSamplerStateInternal(desc);

        notifySamplerStateCreated(desc, state);
    }

    return state;
}

DepthStencilState *RenderStateManager::createDepthStencilState(const DepthStencilStateDesc &desc) const {
    auto state = findCachedState(desc);
    if (state == nullptr) {
        state = createDepthStencilStateInternal(desc);

        notifyDepthStencilStateCreated(desc, state);
    }

    return state;
}

SamplerState *RenderStateManager::getDefaultSamplerState() const {
    if (mDefaultSamplerState == nullptr) {
        mDefaultSamplerState = createSamplerState(SamplerStateDesc());
    }

    return mDefaultSamplerState;
}

DepthStencilState *RenderStateManager::getDefaultDepthStencilState() const {
    if (mDefaultDepthStencilState == nullptr) {
        mDefaultDepthStencilState = createDepthStencilState(DepthStencilStateDesc());
    }

    return mDefaultDepthStencilState;
}

void RenderStateManager::onShutDown() {
    bIsShutdown = true;

    for (auto &pair: mCachedSamplerStateMap) {
        q_delete(pair.value);
    }

    for (auto &pair: mCachedDepthStencilStateMap) {
        q_delete(pair.value);
    }

    // q_delete(mDefaultSamplerState);
    // q_delete(mDefaultDepthStencilState);
}

FGpuPipelineParamInfo *RenderStateManager::createPipelineParamInfoInternal(const FGpuPipelineParamsDesc &desc) const {
    return q_new<FGpuPipelineParamInfo>(desc);
}

SamplerState *RenderStateManager::createSamplerStateInternal(const SamplerStateDesc &desc) const {
    return q_new<SamplerState>(desc);
}

DepthStencilState *RenderStateManager::createDepthStencilStateInternal(const DepthStencilStateDesc &desc) const {
    return q_new<DepthStencilState>(desc);
}

void RenderStateManager::notifySamplerStateCreated(const SamplerStateDesc &desc, SamplerState *state) const {
    mCachedSamplerStateMap.add(desc, state);
}

void RenderStateManager::notifySamplerStateDestroyed(const SamplerStateDesc &desc) const {
    if (!bIsShutdown) {
        mCachedSamplerStateMap.remove(desc);
    }
}

void RenderStateManager::notifyDepthStencilStateCreated(const DepthStencilStateDesc &desc,
                                                        DepthStencilState *state) const {
    mCachedDepthStencilStateMap.add(desc, state);
}

void RenderStateManager::notifyDepthStencilStateDestroyed(const DepthStencilStateDesc &desc) const {
    if (!bIsShutdown) {
        mCachedDepthStencilStateMap.remove(desc);
    }
}

SamplerState *RenderStateManager::findCachedState(const SamplerStateDesc &desc) const {
    auto item = mCachedSamplerStateMap.find(desc);

    if (item != nullptr) {
        return *item;
    }

    return nullptr;
}

DepthStencilState *RenderStateManager::findCachedState(const DepthStencilStateDesc &desc) const {
    auto item = mCachedDepthStencilStateMap.find(desc);

    if (item != nullptr) {
        return *item;
    }

    return nullptr;
}

RenderStateManager &gRenderStateManager() {
    return RenderStateManager::Instance();
}
