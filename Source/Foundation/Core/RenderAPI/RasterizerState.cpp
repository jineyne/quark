//
// Created by jiney on 2024-07-10.
//

#include "RasterizerState.h"

#include "Manager/RenderStateManager.h"

bool RasterizerStateDesc::operator==(const RasterizerStateDesc &rhs) const {
    return polygonMode == rhs.polygonMode &&
           cullMode == rhs.cullMode &&
           depthBias == rhs.depthBias &&
           depthBiasClamp == rhs.depthBiasClamp &&
           slopeScaledDepthBias == rhs.slopeScaledDepthBias &&
           depthClipEnable == rhs.depthClipEnable &&
           scissorEnable == rhs.scissorEnable &&
           multisampleEnable == rhs.multisampleEnable &&
           antialiasedLineEnable == rhs.antialiasedLineEnable;
}

bool RasterizerStateDesc::operator!=(const RasterizerStateDesc &rhs) const {
    return !(rhs == *this);
}

RasterizerState::RasterizerState(const RasterizerStateDesc &info)
        : mInfo(info) {}

RasterizerState *RasterizerState::New(const RasterizerStateDesc &info) {
    return gRenderStateManager().createRasterizerState(info);
}

RasterizerState *RasterizerState::GetDefault() {
    return gRenderStateManager().getDefaultRasterizerState();
}

const RasterizerStateDesc &RasterizerState::getInfo() const {
    return mInfo;
}
