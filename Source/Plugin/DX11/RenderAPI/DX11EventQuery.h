#pragma once

#include "DX11Prerequisites.h"

#include "RenderAPI/EventQuery.h"

class DX11_EXPORT DX11EventQuery : public EventQuery {
private:
    ID3D11Query* mQuery = nullptr;
    ID3D11DeviceContext* mContext;

public:
    DX11EventQuery(uint32_t deviceIdx = 0);
    ~DX11EventQuery();

public:
    void begin(CommandBuffer *cb = nullptr) override;
    bool isReady() const override;
};
