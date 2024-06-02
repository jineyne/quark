#include "DX11BlendState.h"

#include "RenderApi/DX11Device.h"
#include "RenderApi/DX11RenderAPI.h"

DX11BlendState::DX11BlendState(const BlendStateDesc &desc, UINT32 id) : BlendState(desc) {
    D3D11_BLEND_DESC blendStateDesc;
    ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

    blendStateDesc.RenderTarget[0].BlendEnable = desc.renderTargetDesc.blendEnable;
    //blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

    DX11RenderAPI* rs = static_cast<DX11RenderAPI*>(RenderAPI::InstancePtr());
    DX11Device *device = rs->getPrimaryDevice();
    HRESULT hr = device->getDevice()->CreateBlendState(&blendStateDesc, &mBlendState);

    if(FAILED(hr) || device->hasError()) {
        String errorDescription = device->getErrorDescription();
        EXCEPT(LogDx11, RenderAPIException, TEXT("Cannot create blend state.\nError Description: %ls"), *errorDescription);
    }
}

DX11BlendState::~DX11BlendState() {
    SAFE_RELEASE(mBlendState);
}
