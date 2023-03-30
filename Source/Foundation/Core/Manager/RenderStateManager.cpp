#include "RenderStateManager.h"

FGpuPipelineParamInfo *FRenderStateManager::createPipelineParamInfo(const FGpuPipelineParamsDesc &desc) const {
    return createPipelineParamInfoInternal(desc);
}

FSamplerState *FRenderStateManager::createSamplerState(const FSamplerStateDesc &desc) const {
    auto state = findCachedState(desc);
    if (state == nullptr) {
        state = createSamplerStateInternal(desc);

        notifySamplerStateCreated(desc, state);
    }

    return state;
}

FDepthStencilState *FRenderStateManager::createDepthStencilState(const FDepthStencilStateDesc &desc) const {
    auto state = findCachedState(desc);
    if (state == nullptr) {
        state = createDepthStencilStateInternal(desc);

        notifyDepthStencilStateCreated(desc, state);
    }

    return state;
}

FSamplerState *FRenderStateManager::getDefaultSamplerState() const {
    if (mDefaultSamplerState == nullptr) {
        mDefaultSamplerState = createSamplerState(FSamplerStateDesc());
    }

    return mDefaultSamplerState;
}

FDepthStencilState *FRenderStateManager::getDefaultDepthStencilState() const {
    if (mDefaultDepthStencilState == nullptr) {
        mDefaultDepthStencilState = createDepthStencilState(FDepthStencilStateDesc());
    }

    return mDefaultDepthStencilState;
}

void FRenderStateManager::onShutDown() {
    for (auto &pair : mCachedSamplerStateMap) {
        q_delete(pair.second);
    }

    for (auto &pair : mCachedDepthStencilStateMap) {
        q_delete(pair.second);
    }

    q_delete(mDefaultSamplerState);
    q_delete(mDefaultDepthStencilState);
}

FGpuPipelineParamInfo *FRenderStateManager::createPipelineParamInfoInternal(const FGpuPipelineParamsDesc &desc) const {
    return q_new<FGpuPipelineParamInfo>(desc);
}

FSamplerState *FRenderStateManager::createSamplerStateInternal(const FSamplerStateDesc &desc) const {
    return q_new<FSamplerState>(desc);
}

FDepthStencilState *FRenderStateManager::createDepthStencilStateInternal(const FDepthStencilStateDesc &desc) const {
    return q_new<FDepthStencilState>(desc);
}

void FRenderStateManager::notifySamplerStateCreated(const FSamplerStateDesc &desc, FSamplerState *state) const {
    mCachedSamplerStateMap.add(desc, state);
}

void FRenderStateManager::notifySamplerStateDestroyed(const FSamplerStateDesc &desc) const {
    mCachedSamplerStateMap.remove(desc);
}

void FRenderStateManager::notifyDepthStencilStateCreated(const FDepthStencilStateDesc &desc,
                                                         FDepthStencilState *state) const {
    mCachedDepthStencilStateMap.add(desc, state);
}

void FRenderStateManager::notifyDepthStencilStateDestroyed(const FDepthStencilStateDesc &desc) const {
    mCachedDepthStencilStateMap.remove(desc);
}

FSamplerState *FRenderStateManager::findCachedState(const FSamplerStateDesc &desc) const {
    auto item = mCachedSamplerStateMap.find(desc);

    if (item != nullptr) {
        return *item;
    }

    return nullptr;
}

FDepthStencilState *FRenderStateManager::findCachedState(const FDepthStencilStateDesc &desc) const {
    auto item = mCachedDepthStencilStateMap.find(desc);

    if (item != nullptr) {
        return *item;
    }

    return nullptr;
}

FRenderStateManager &gRenderStateManager() {
    return FRenderStateManager::Instance();
}
