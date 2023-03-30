#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/DepthStencilState.h"

class DX11_EXPORT FDX11DepthStencilState : public FDepthStencilState {
private:
    ID3D11DepthStencilState* mDepthStencilState = nullptr;

public:
    ~FDX11DepthStencilState();

public:
    ID3D11DepthStencilState *getInternal() const { return mDepthStencilState; }

protected:
    FDX11DepthStencilState(const FDepthStencilStateDesc &desc);

    void createInternal() override;

private:
    friend class FDX11RenderStateManager;
};
