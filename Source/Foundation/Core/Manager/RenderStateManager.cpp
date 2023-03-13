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

FSamplerState *FRenderStateManager::getDefaultSamplerState() const {
    if (mDefaultSamplerState == nullptr) {
        mDefaultSamplerState = createSamplerState(FSamplerStateDesc());
    }

    return mDefaultSamplerState;
}

void FRenderStateManager::onShutDown() {
    delete mDefaultSamplerState;
}

FGpuPipelineParamInfo *FRenderStateManager::createPipelineParamInfoInternal(const FGpuPipelineParamsDesc &desc) const {
    return new FGpuPipelineParamInfo(desc);
}

FSamplerState *FRenderStateManager::createSamplerStateInternal(const FSamplerStateDesc &desc) const {
    return new FSamplerState(desc);
}

void FRenderStateManager::notifySamplerStateCreated(const FSamplerStateDesc &desc, FSamplerState *state) const {
    mCachedSamplerStateMap.add(desc, state);
}

void FRenderStateManager::notifySamplerStateDestroyed(const FSamplerStateDesc &desc) const {
    mCachedSamplerStateMap.remove(desc);
}

FSamplerState *FRenderStateManager::findCachedState(const FSamplerStateDesc &desc) const {
    auto item = mCachedSamplerStateMap.find(desc);

    if (item != nullptr) {
        return *item;
    }

    return nullptr;
}

FRenderStateManager &gRenderStateManager() {
    return FRenderStateManager::Instance();
}
