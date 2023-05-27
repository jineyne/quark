#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/SamplerState.h"

class DX11_EXPORT DX11SamplerState : public SamplerState {
private:
    ID3D11SamplerState *mSamplerState = nullptr;

public:
    virtual ~DX11SamplerState();

public:
    ID3D11SamplerState *getInternal() const { return mSamplerState; }

protected:
    DX11SamplerState(const SamplerStateDesc &desc);

    void createInternal() override;

private:
    friend class DX11RenderStateManager;
};