#pragma once

#include "DX11Prerequisites.h"
#include "Manager/RenderWindowManager.h"
#include "DX11RenderWindowManager.g.h"

QCLASS()
class DX11_EXPORT FDX11RenderWindowManager : public FRenderWindowManager {
    GENERATED_BODY()

public:
    void update() override;

protected:
    FRenderWindow *createInternal(const FRenderWindowDesc &desc, uint32_t windowId, FRenderWindow *parent) override;
};

