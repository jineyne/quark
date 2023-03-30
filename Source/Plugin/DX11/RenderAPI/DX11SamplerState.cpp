#include "DX11SamplerState.h"

#include "RenderAPI/DX11Device.h"
#include "RenderAPI/DX11RenderAPI.h"

FDX11SamplerState::FDX11SamplerState(const FSamplerStateDesc &desc) : FSamplerState(desc) {
    // TODO: MOVE TO INITIALIZE FUNC
    createInternal();
}

FDX11SamplerState::~FDX11SamplerState() {
    SAFE_RELEASE(mSamplerState);
}

void FDX11SamplerState::createInternal() {
    FSamplerState::createInternal();

    // TODO: parse from desc! but not now!

    D3D11_SAMPLER_DESC samplerState{};
    samplerState.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerState.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerState.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerState.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerState.MipLODBias = 0.0f;
    samplerState.MaxAnisotropy = 1;
    samplerState.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerState.BorderColor[0] = 0;
    samplerState.BorderColor[1] = 0;
    samplerState.BorderColor[2] = 0;
    samplerState.BorderColor[3] = 0;
    samplerState.MinLOD = 0;
    samplerState.MaxLOD = D3D11_FLOAT32_MAX;

    auto *rapi = static_cast<FDX11RenderAPI *>(FRenderAPI::InstancePtr());
    auto device = rapi->getPrimaryDevice();
    HR(device->getDevice()->CreateSamplerState(&samplerState, &mSamplerState));

}
