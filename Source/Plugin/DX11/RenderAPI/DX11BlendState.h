#pragma once

#include "Dx11Prerequisites.h"
#include <RenderAPI/BlendState.h>

class DX11BlendState : public BlendState {
private:
    ID3D11BlendState* mBlendState = nullptr;

protected:
    DX11BlendState(const BlendStateDesc& desc, UINT32 id);

public:
    ~DX11BlendState();

public:
    /**	Returns the internal DX11 blend state object. */
    ID3D11BlendState* getInternal() const { return mBlendState; }

private:
    friend class DX11RenderStateManager;
};
