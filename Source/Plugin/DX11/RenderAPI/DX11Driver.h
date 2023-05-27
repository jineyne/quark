#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/VideoModeInfo.h"

class DX11_EXPORT DX11Driver {
private:
    uint32_t mAdapterIndex;
    uint32_t mOutputCount;

    DXGI_ADAPTER_DESC mAdapterId;
    IDXGIAdapter *mDXGIAdapter = nullptr;

    VideoModeInfo *mVideoModeInfo = nullptr;

public:
    DX11Driver(uint32_t adapterIndex, IDXGIAdapter *dxgiAdapter);
    ~DX11Driver();

public:
    static DX11Driver *New(uint32_t adapterIndex, IDXGIAdapter *dxgiAdapter);

public:
    DXGI_OUTPUT_DESC getOutputDesc(uint32_t idx) const;

    String getDriverName() const { return String::Empty; }
    uint32_t getAdapterIndex() const { return mAdapterIndex; }
    uint32_t getOutputCount() const { return mOutputCount; }
    const DXGI_ADAPTER_DESC &getAdapterId() const { return mAdapterId; }
    IDXGIAdapter *getAdapter() const { return mDXGIAdapter; }
    VideoModeInfo *getVideoModeInfo() const { return mVideoModeInfo; }
};
