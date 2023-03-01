#include "SamplerState.h"

FSamplerStateDesc::FSamplerStateDesc() {}

bool FSamplerStateDesc::operator==(const FSamplerStateDesc &rhs) const {
    return addressMode == rhs.addressMode && minFilter == rhs.minFilter && magFilter == rhs.magFilter &&
           mipFilter == rhs.mipFilter && maxAniso == rhs.maxAniso && mipmapBias == rhs.mipmapBias &&
           mipMin == rhs.mipMin && mipMax == rhs.mipMax && borderColor == rhs.borderColor &&
           comparisonFunc == rhs.comparisonFunc;
}

FSamplerState::FSamplerState(const FSamplerStateDesc &desc) {

}

FSamplerState::~FSamplerState() {

}

FSamplerState *FSamplerState::New(const FSamplerStateDesc &desc) {
    return nullptr;
}

FSamplerState *FSamplerState::GetDefault() {
    return nullptr;
}

size_t FSamplerState::GenerateHash(const FSamplerStateDesc &desc) {
    return 0;
}

FilterOptions FSamplerState::getTextureFiltering(FilterType ft) const {
    return FilterOptions::Linear;
}
