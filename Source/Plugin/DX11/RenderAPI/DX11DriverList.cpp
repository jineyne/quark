#include "DX11DriverList.h"

#include "DX11Driver.h"
#include "Exception/Exception.h"

DX11DriverList::DX11DriverList(IDXGIFactory1 *dxgiFactory) {
    enumerate(dxgiFactory);
}

DX11DriverList::~DX11DriverList() {
    for (auto driver : mDriverList) {
        q_delete(driver);
    }

    mDriverList.clear();
}

DX11DriverList *DX11DriverList::New(IDXGIFactory1 *dxgiFactory) {
    return q_new<DX11DriverList>(dxgiFactory);
}

uint32_t DX11DriverList::count() const {
    return mDriverList.length();
}

DX11Driver *DX11DriverList::get(uint32_t idx) const {
    return mDriverList[idx];
}

DX11Driver *DX11DriverList::get(const String &name) const {
    for (auto it : mDriverList) {
        if (it->getDriverName() == name) {
            return it;
        }
    }

    return nullptr;
}

void DX11DriverList::enumerate(IDXGIFactory1 *dxgiFactory) {
    uint32_t idx = 0;
    IDXGIAdapter *dxgiAdapter = nullptr;
    HRESULT hr;

    while ((hr = dxgiFactory->EnumAdapters(idx, &dxgiAdapter)) != DXGI_ERROR_NOT_FOUND) {
        if (FAILED(hr)) {
            SAFE_RELEASE(dxgiAdapter);
            EXCEPT(LogDX11, RenderAPIException, TEXT("Enumerating adapters failed."));
        }

        mDriverList.add(DX11Driver::New(idx, dxgiAdapter));

        SAFE_RELEASE(dxgiAdapter);
        idx++;
    }
}
