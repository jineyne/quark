#pragma once

#include "DX11Prerequisites.h"

#include "RenderAPI/EventQuery.h"

class DX11_EXPORT FDX11EventQuery : public FEventQuery {
private:
    ID3D11Query* mQuery = nullptr;
    ID3D11DeviceContext* mContext;

public:
    FDX11EventQuery(uint32_t deviceIdx = 0);
    ~FDX11EventQuery();

public:
    void begin(FCommandBuffer *cb = nullptr) override;
    bool isReady() const override;
};
