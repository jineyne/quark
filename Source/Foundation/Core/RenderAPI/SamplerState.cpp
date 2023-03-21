#include "SamplerState.h"
#include "Manager/RenderStateManager.h"

FSamplerStateDesc::FSamplerStateDesc() {}

bool FSamplerStateDesc::operator==(const FSamplerStateDesc &rhs) const {
    return addressMode == rhs.addressMode && minFilter == rhs.minFilter && magFilter == rhs.magFilter &&
           mipFilter == rhs.mipFilter && maxAniso == rhs.maxAniso && mipmapBias == rhs.mipmapBias &&
           mipMin == rhs.mipMin && mipMax == rhs.mipMax && borderColor == rhs.borderColor &&
           comparisonFunc == rhs.comparisonFunc;
}

FSamplerState::FSamplerState(const FSamplerStateDesc &desc) : mDesc(desc) {
}

FSamplerState::~FSamplerState() {
    FRenderStateManager::Instance().notifySamplerStateDestroyed(mDesc);
}

FSamplerState *FSamplerState::New(const FSamplerStateDesc &desc) {
    return FRenderStateManager::Instance().createSamplerState(desc);
}

FSamplerState *FSamplerState::GetDefault() {
    return FRenderStateManager::Instance().getDefaultSamplerState();
}

size_t FSamplerState::GenerateHash(const FSamplerStateDesc &desc) {
    size_t hash = 0;

    CombineHash(hash, (uint32_t)desc.addressMode.u);
    CombineHash(hash, (uint32_t)desc.addressMode.v);
    CombineHash(hash, (uint32_t)desc.addressMode.w);
    CombineHash(hash, (uint32_t)desc.minFilter);
    CombineHash(hash, (uint32_t)desc.magFilter);
    CombineHash(hash, (uint32_t)desc.mipFilter);
    CombineHash(hash, desc.maxAniso);
    CombineHash(hash, desc.mipmapBias);
    CombineHash(hash, desc.mipMin);
    CombineHash(hash, desc.mipMax);
    CombineHash(hash, desc.borderColor);
    CombineHash(hash, (uint32_t)desc.comparisonFunc);

    return hash;
}

EFilterOptions FSamplerState::getTextureFiltering(FilterType ft) const {
    switch (ft) {
        case FilterType::Min:
        default:
            return mDesc.minFilter;

        case FilterType::Mag:
            return mDesc.magFilter;

        case FilterType::Mip:
            return mDesc.mipFilter;
    }
}
