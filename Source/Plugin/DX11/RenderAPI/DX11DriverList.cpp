#include "DX11DriverList.h"

#include "DX11Driver.h"
#include "Exception/Exception.h"

FDX11DriverList::FDX11DriverList(IDXGIFactory1 *dxgiFactory) {
    enumerate(dxgiFactory);
}

FDX11DriverList::~FDX11DriverList() {
    for (auto driver : mDriverList) {
        q_delete(driver);
    }

    mDriverList.clear();
}

FDX11DriverList *FDX11DriverList::New(IDXGIFactory1 *dxgiFactory) {
    return q_new<FDX11DriverList>(dxgiFactory);
}

uint32_t FDX11DriverList::count() const {
    return mDriverList.length();
}

FDX11Driver *FDX11DriverList::get(uint32_t idx) const {
    return mDriverList[idx];
}

FDX11Driver *FDX11DriverList::get(const FString &name) const {
    for (auto it : mDriverList) {
        if (it->getDriverName() == name) {
            return it;
        }
    }

    return nullptr;
}

void FDX11DriverList::enumerate(IDXGIFactory1 *dxgiFactory) {
    uint32_t idx = 0;
    IDXGIAdapter *dxgiAdapter = nullptr;
    HRESULT hr;

    while ((hr = dxgiFactory->EnumAdapters(idx, &dxgiAdapter)) != DXGI_ERROR_NOT_FOUND) {
        if (FAILED(hr)) {
            SAFE_RELEASE(dxgiAdapter);
            EXCEPT(FLogDX11, RenderAPIException, TEXT("Enumerating adapters failed."));
        }

        mDriverList.add(FDX11Driver::New(idx, dxgiAdapter));

        SAFE_RELEASE(dxgiAdapter);
        idx++;
    }
}
