#pragma once

#include "DX11Prerequisites.h"

class DX11_EXPORT DX11DriverList {
private:
    TArray<DX11Driver *> mDriverList;

public:
    DX11DriverList(IDXGIFactory1 *dxgiFactory);
    ~DX11DriverList();

public:
    static DX11DriverList *New(IDXGIFactory1 *dxgiFactory);

public:
    uint32_t count() const;

    DX11Driver *get(uint32_t idx) const;
    DX11Driver *get(const String &name) const;

private:
    void enumerate(IDXGIFactory1 *dxgiFactory);
};
