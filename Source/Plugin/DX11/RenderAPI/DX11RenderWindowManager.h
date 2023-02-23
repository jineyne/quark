#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/RenderWindowManager.h"
#include "DX11RenderWindowManager.g.h"

QCLASS()
class DX11_EXPORT FDX11RenderWindowManager : public FRenderWindowManager {
    GENERATED_BODY()

protected:
    FRenderWindow *createInternal(const FRenderWindowDesc &desc, uint32_t windowId, FRenderWindow *parent) override;
};

