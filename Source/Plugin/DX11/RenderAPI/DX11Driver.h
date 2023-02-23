#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/VideoModeInfo.h"

class DX11_EXPORT FDX11Driver {
private:
    uint32_t mAdapterIndex;
    uint32_t mOutputCount;

    DXGI_ADAPTER_DESC mAdapterId;
    IDXGIAdapter *mDXGIAdapter = nullptr;

    FVideoModeInfo *mVideoModeInfo = nullptr;

public:
    FDX11Driver(uint32_t adapterIndex, IDXGIAdapter *dxgiAdapter);
    ~FDX11Driver();

public:
    static FDX11Driver *New(uint32_t adapterIndex, IDXGIAdapter *dxgiAdapter);

public:
    DXGI_OUTPUT_DESC getOutputDesc(uint32_t idx) const;

    FString getDriverName() const { return FString::Empty; }
    uint32_t getAdapterIndex() const { return mAdapterIndex; }
    uint32_t getOutputCount() const { return mOutputCount; }
    const DXGI_ADAPTER_DESC &getAdapterId() const { return mAdapterId; }
    IDXGIAdapter *getAdapter() const { return mDXGIAdapter; }
    FVideoModeInfo *getVideoModeInfo() const { return mVideoModeInfo; }
};
