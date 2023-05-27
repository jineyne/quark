#include "DX11DepthStencilState.h"
#include "Utility/DX11Mapper.h"
#include "RenderAPI/DX11Device.h"
#include "RenderAPI/DX11RenderAPI.h"

DX11DepthStencilState::DX11DepthStencilState(const DepthStencilStateDesc &desc)
        : DepthStencilState(desc) {
    createInternal();
}

DX11DepthStencilState::~DX11DepthStencilState() {
    SAFE_RELEASE(mDepthStencilState);
}

void DX11DepthStencilState::createInternal() {
    DepthStencilState::createInternal();

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    bool depthEnable = getDepthWriteEnable() || getDepthReadEnable();
    ECompareFunction compareFunc;
    if (getDepthReadEnable()) {
        compareFunc = getDepthComparisonFunc();
    } else {
        compareFunc = ECompareFunction::AlwaysPass;
    }

    depthStencilDesc.BackFace.StencilPassOp = DX11Mapper::Get(getStencilBackPassOp());
    depthStencilDesc.BackFace.StencilFailOp = DX11Mapper::Get(getStencilBackFailOp());
    depthStencilDesc.BackFace.StencilDepthFailOp = DX11Mapper::Get(getStencilBackZFailOp());
    depthStencilDesc.BackFace.StencilFunc = DX11Mapper::Get(getStencilBackCompFunc());
    depthStencilDesc.FrontFace.StencilPassOp = DX11Mapper::Get(getStencilFrontPassOp());
    depthStencilDesc.FrontFace.StencilFailOp = DX11Mapper::Get(getStencilFrontFailOp());
    depthStencilDesc.FrontFace.StencilDepthFailOp = DX11Mapper::Get(getStencilFrontZFailOp());
    depthStencilDesc.FrontFace.StencilFunc = DX11Mapper::Get(getStencilFrontCompFunc());
    depthStencilDesc.DepthEnable = depthEnable;
    depthStencilDesc.DepthWriteMask = getDepthWriteEnable() ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = DX11Mapper::Get(compareFunc);
    depthStencilDesc.StencilEnable = getStencilEnable();
    depthStencilDesc.StencilReadMask = getStencilReadMask();
    depthStencilDesc.StencilWriteMask = getStencilWriteMask();

    auto *rapi = static_cast<DX11RenderAPI *>(RenderAPI::InstancePtr());
    auto device = rapi->getPrimaryDevice();
    HR(device->getDevice()->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState));
}
