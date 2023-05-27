#pragma once

#include "DX11Prerequisites.h"
#include "Manager/RenderWindowManager.h"
#include "DX11RenderWindowManager.g.h"

QCLASS()
class DX11_EXPORT DX11RenderWindowManager : public RenderWindowManager {
    GENERATED_BODY()

public:
    void update() override;

protected:
    RenderWindow *createInternal(const RenderWindowDesc &desc, uint32_t windowId, RenderWindow *parent) override;
};

