#include "DX11Driver.h"
#include "DX11VideoModeInfo.h"
#include "Exception/Exception.h"

FDX11Driver::FDX11Driver(uint32_t adapterIndex, IDXGIAdapter *dxgiAdapter)
        : mAdapterIndex(adapterIndex), mDXGIAdapter(dxgiAdapter) {
    assert(mDXGIAdapter != nullptr);

    if (mDXGIAdapter) {
        mDXGIAdapter->AddRef();
    }


    uint32_t idx = 0;
    IDXGIOutput *output = nullptr;
    while (mDXGIAdapter->EnumOutputs(idx, &output) != DXGI_ERROR_NOT_FOUND) {
        idx++;
        SAFE_RELEASE(output);
    }

    mOutputCount = idx;
    mVideoModeInfo = q_new<FDX11VideoModeInfo>(mDXGIAdapter);
}

FDX11Driver::~FDX11Driver() {
    q_delete(mVideoModeInfo);
    SAFE_RELEASE(mDXGIAdapter);
}

FDX11Driver *FDX11Driver::New(uint32_t adapterIndex, IDXGIAdapter *dxgiAdapter) {
    return q_new<FDX11Driver>(adapterIndex, dxgiAdapter);
}

DXGI_OUTPUT_DESC FDX11Driver::getOutputDesc(uint32_t idx) const {
    DXGI_OUTPUT_DESC desc;

    IDXGIOutput *output = nullptr;
    if (mDXGIAdapter->EnumOutputs(idx, &output) == DXGI_ERROR_NOT_FOUND) {
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Cannot find output with the specified gIBO: %d"), idx);
    }

    output->GetDesc(&desc);
    SAFE_RELEASE(output);

    return desc;
}
