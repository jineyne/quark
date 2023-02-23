#include "DX11VideoModeInfo.h"
#include "Exception/Exception.h"

FDX11VideoMode::FDX11VideoMode(uint32_t width, uint32_t height, float refreshRate, uint32_t outputIdx,
                               uint32_t numerator, uint32_t denominator, DXGI_MODE_DESC mode)
        : FVideoMode(width, height, refreshRate), numerator(numerator), denominator(denominator), mode(mode) {

}

FDX11VideoOutputInfo::FDX11VideoOutputInfo(IDXGIOutput *output, uint32_t outputIdx) {
    DXGI_OUTPUT_DESC desc{};
    output->GetDesc(&desc);

    int nLen = wcslen(desc.DeviceName);
    char *pstr = (char *) malloc(sizeof(char) * nLen + 1);
    wcstombs(pstr, desc.DeviceName, nLen + 1);

    mName = pstr;

    free(pstr);

    uint32_t modeCount = 0;
    HRESULT hr = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &modeCount, nullptr);
    if (FAILED(hr)) {
        SAFE_RELEASE(output);
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Error while enumerating adapter output video modes."));
    }

    TArray<DXGI_MODE_DESC> modeDescList(modeCount);
    hr = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &modeCount, modeDescList.getData());
    if (FAILED(hr)) {
        SAFE_RELEASE(output);
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Error while enumerating adapter output video modes."));
    }

    for (auto displayMode : modeDescList) {
        bool foundVdMode = false;

        for (auto videoMode : mVideoModeList) {
            auto vm = static_cast<FDX11VideoMode *>(videoMode);

            if (vm->width == displayMode.Width && vm->height == displayMode.Height &&
                vm->numerator == displayMode.RefreshRate.Numerator &&
                vm->denominator == displayMode.RefreshRate.Denominator) {
                foundVdMode = true;
                break;
            }
        }

        if (!foundVdMode) {
            float refreshRate = displayMode.RefreshRate.Numerator / (float) displayMode.RefreshRate.Denominator;
            auto videoMode = new FDX11VideoMode(displayMode.Width, displayMode.Height, refreshRate, outputIdx,
                                                displayMode.RefreshRate.Numerator,
                                                displayMode.RefreshRate.Denominator, displayMode);
            videoMode->isCustom = false;
            mVideoModeList.add(videoMode);
        }
    }

    HMONITOR hMonitor = desc.Monitor;
    MONITORINFOEX monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(hMonitor, &monitorInfo);

    DEVMODE devMode;
    devMode.dmSize = sizeof(DEVMODE);
    devMode.dmDriverExtra = 0;
    EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode);

    DXGI_MODE_DESC currentMode;
    currentMode.Width = devMode.dmPelsWidth;
    currentMode.Height = devMode.dmPelsHeight;
    bool useDefaultRefreshRate = 1 == devMode.dmDisplayFrequency || 0 == devMode.dmDisplayFrequency;
    currentMode.RefreshRate.Numerator = useDefaultRefreshRate ? 0 : devMode.dmDisplayFrequency;
    currentMode.RefreshRate.Denominator = useDefaultRefreshRate ? 0 : 1;
    currentMode.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    currentMode.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    currentMode.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    DXGI_MODE_DESC nearestMode{};

    output->FindClosestMatchingMode(&currentMode, &nearestMode, nullptr);

    float refreshRate = nearestMode.RefreshRate.Numerator / (float) nearestMode.RefreshRate.Denominator;
    mDesktopVideoMode = new FDX11VideoMode(nearestMode.Width, nearestMode.Height, refreshRate, outputIdx,
                                           nearestMode.RefreshRate.Numerator, nearestMode.RefreshRate.Denominator,
                                           nearestMode);
    mDesktopVideoMode->isCustom = false;
}

IDXGIOutput *FDX11VideoOutputInfo::getDxgiOutput() const {
    return mDXGIOutput;
}

FDX11VideoModeInfo::FDX11VideoModeInfo(IDXGIAdapter *dxgiAdapter) {
    int32_t idx = 0;
    IDXGIOutput *output = nullptr;
    while (dxgiAdapter->EnumOutputs(idx, &output) != DXGI_ERROR_NOT_FOUND) {
        mVideoOutputList.add(new FDX11VideoOutputInfo(output, idx));
        idx++;
    }
}