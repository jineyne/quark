#include "DX11DepthStencilState.h"
#include "Utility/DX11Mapper.h"
#include "RenderAPI/DX11Device.h"
#include "RenderAPI/DX11RenderAPI.h"

FDX11DepthStencilState::FDX11DepthStencilState(const FDepthStencilStateDesc &desc)
        : FDepthStencilState(desc) {
    createInternal();
}

FDX11DepthStencilState::~FDX11DepthStencilState() {
    SAFE_RELEASE(mDepthStencilState);
}

void FDX11DepthStencilState::createInternal() {
    FDepthStencilState::createInternal();

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    bool depthEnable = getDepthWriteEnable() || getDepthReadEnable();
    ECompareFunction compareFunc;
    if (getDepthReadEnable()) {
        compareFunc = getDepthComparisonFunc();
    } else {
        compareFunc = ECompareFunction::AlwaysPass;
    }

    depthStencilDesc.BackFace.StencilPassOp = FDX11Mapper::Get(getStencilBackPassOp());
    depthStencilDesc.BackFace.StencilFailOp = FDX11Mapper::Get(getStencilBackFailOp());
    depthStencilDesc.BackFace.StencilDepthFailOp = FDX11Mapper::Get(getStencilBackZFailOp());
    depthStencilDesc.BackFace.StencilFunc = FDX11Mapper::Get(getStencilBackCompFunc());
    depthStencilDesc.FrontFace.StencilPassOp = FDX11Mapper::Get(getStencilFrontPassOp());
    depthStencilDesc.FrontFace.StencilFailOp = FDX11Mapper::Get(getStencilFrontFailOp());
    depthStencilDesc.FrontFace.StencilDepthFailOp = FDX11Mapper::Get(getStencilFrontZFailOp());
    depthStencilDesc.FrontFace.StencilFunc = FDX11Mapper::Get(getStencilFrontCompFunc());
    depthStencilDesc.DepthEnable = depthEnable;
    depthStencilDesc.DepthWriteMask = getDepthWriteEnable() ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = FDX11Mapper::Get(compareFunc);
    depthStencilDesc.StencilEnable = getStencilEnable();
    depthStencilDesc.StencilReadMask = getStencilReadMask();
    depthStencilDesc.StencilWriteMask = getStencilWriteMask();

    auto *rapi = static_cast<FDX11RenderAPI *>(FRenderAPI::InstancePtr());
    auto device = rapi->getPrimaryDevice();
    HR(device->getDevice()->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState));
}
