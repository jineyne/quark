#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/RasterizerState.h"

class DX11RasterizerState : public RasterizerState {
private:
    ID3D11RasterizerState *mRasterizerState;

public:
    DX11RasterizerState(const RasterizerStateDesc &info);
    virtual ~DX11RasterizerState() override;

public:
    ID3D11RasterizerState *getInternal() const;
};
