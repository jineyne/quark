#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/SamplerState.h"

class DX11_EXPORT FDX11SamplerState : public FSamplerState {
private:
    ID3D11SamplerState *mSamplerState = nullptr;

public:
    virtual ~FDX11SamplerState();

public:
    ID3D11SamplerState *getInternal() const { return mSamplerState; }

protected:
    FDX11SamplerState(const FSamplerStateDesc &desc);

    void createInternal() override;

private:
    friend class DX11RenderStateManager;
};