//
// Created by jiney on 2024-07-10.
//

#include "DX11RasterizerState.h"
#include "RenderAPI/RenderAPI.h"
#include "Utility/DX11Mapper.h"

#include "DX11Device.h"
#include "DX11RenderAPI.h"

DX11RasterizerState::DX11RasterizerState(const RasterizerStateDesc &info) : RasterizerState(info) {
    int32_t scaledDepthBias = Math::FloorToInt(-mInfo.depthBias * float(1 << 24));

    D3D11_RASTERIZER_DESC desc{};
    desc.AntialiasedLineEnable = mInfo.antialiasedLineEnable;
    desc.CullMode = DX11Mapper::Get(mInfo.cullMode);
    desc.DepthBias = scaledDepthBias;
    desc.DepthBiasClamp = mInfo.depthBiasClamp;
    desc.DepthClipEnable = mInfo.depthClipEnable;
    desc.FillMode = DX11Mapper::Get(mInfo.polygonMode);
    desc.MultisampleEnable = mInfo.multisampleEnable;
    desc.ScissorEnable = mInfo.scissorEnable;
    desc.SlopeScaledDepthBias = mInfo.slopeScaledDepthBias;
    desc.FrontCounterClockwise = false;

    DX11RenderAPI* rapi = static_cast<DX11RenderAPI*>(RenderAPI::InstancePtr());
    auto device = rapi->getPrimaryDevice();
    HRESULT hr = device->getDevice()->CreateRasterizerState(&desc, &mRasterizerState);

    if (FAILED(hr)) {
        String error = device->getErrorDescription();
        EXCEPT(LogDX11, RenderAPIException, TEXT("Cannot create rasterizer state. error: %ls"), *error);
    }
}

DX11RasterizerState::~DX11RasterizerState() {
    SAFE_RELEASE(mRasterizerState);
}

ID3D11RasterizerState *DX11RasterizerState::getInternal() const {
    return mRasterizerState;
}
