#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/DepthStencilState.h"

class DX11_EXPORT DX11DepthStencilState : public DepthStencilState {
private:
    ID3D11DepthStencilState* mDepthStencilState = nullptr;

public:
    ~DX11DepthStencilState();

public:
    ID3D11DepthStencilState *getInternal() const { return mDepthStencilState; }

protected:
    DX11DepthStencilState(const DepthStencilStateDesc &desc);

    void createInternal() override;

private:
    friend class DX11RenderStateManager;
};
