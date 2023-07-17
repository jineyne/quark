#include "SamplerState.h"
#include "Manager/RenderStateManager.h"

UVWAddressingMode::UVWAddressingMode()
        : u(ETextureAddressingMode::Wrap), v(ETextureAddressingMode::Wrap), w(ETextureAddressingMode::Wrap) {
}

bool UVWAddressingMode::operator==(const UVWAddressingMode &rhs) const {
    return u == rhs.u && v == rhs.v && w == rhs.w;
}

SamplerStateDesc::SamplerStateDesc() {}

bool SamplerStateDesc::operator==(const SamplerStateDesc &rhs) const {
    return addressMode == rhs.addressMode && minFilter == rhs.minFilter && magFilter == rhs.magFilter &&
           mipFilter == rhs.mipFilter && maxAniso == rhs.maxAniso && mipmapBias == rhs.mipmapBias &&
           mipMin == rhs.mipMin && mipMax == rhs.mipMax && borderColor == rhs.borderColor &&
           comparisonFunc == rhs.comparisonFunc;
}

SamplerState::SamplerState(const SamplerStateDesc &desc) : mDesc(desc) {
}

SamplerState::~SamplerState() {
    RenderStateManager::Instance().notifySamplerStateDestroyed(mDesc);
}

SamplerState *SamplerState::New(const SamplerStateDesc &desc) {
    return RenderStateManager::Instance().createSamplerState(desc);
}

SamplerState *SamplerState::GetDefault() {
    return RenderStateManager::Instance().getDefaultSamplerState();
}

size_t SamplerState::GenerateHash(const SamplerStateDesc &desc) {
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

EFilterOptions SamplerState::getTextureFiltering(EFilterType ft) const {
    switch (ft) {
        case EFilterType::Min:
        default:
            return mDesc.minFilter;

        case EFilterType::Mag:
            return mDesc.magFilter;

        case EFilterType::Mip:
            return mDesc.mipFilter;
    }
}
