#pragma once

#include "DX11Prerequisites.h"

class DX11_EXPORT FDX11DriverList {
private:
    TArray<FDX11Driver *> mDriverList;

public:
    FDX11DriverList(IDXGIFactory1 *dxgiFactory);
    ~FDX11DriverList();

public:
    static FDX11DriverList *New(IDXGIFactory1 *dxgiFactory);

public:
    uint32_t count() const;

    FDX11Driver *get(uint32_t idx) const;
    FDX11Driver *get(const FString &name) const;

private:
    void enumerate(IDXGIFactory1 *dxgiFactory);
};
