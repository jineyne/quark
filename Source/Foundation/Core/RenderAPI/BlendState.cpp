#include "BlendState.h"
#include "Manager/RenderStateManager.h"

bool RenderTargetBlendStateDesc::operator==(const RenderTargetBlendStateDesc &rhs) const {
    return blendEnable == rhs.blendEnable;
}

size_t BlendStateDesc::GenerateHash(const BlendStateDesc &desc) {
    size_t hash = 0;

    CombineHash(hash, desc.renderTargetDesc.blendEnable);

    return hash;
}

bool BlendStateDesc::operator==(const BlendStateDesc &rhs) const {
    return renderTargetDesc == rhs.renderTargetDesc;
}

BlendState::BlendState(const BlendStateDesc &desc) : mDesc(desc) { }

BlendState::~BlendState() {
    RenderStateManager::Instance().notifyBlendStateDestroyed(mDesc);
}

BlendState *BlendState::New(const BlendStateDesc &desc) {
    return RenderStateManager::Instance().createBlendState(desc);
}

BlendState *BlendState::GetDefault() {
    return RenderStateManager::Instance().getDefaultBlendState();
}
