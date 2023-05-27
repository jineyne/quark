#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/VideoModeInfo.h"

struct PLUGIN_EXPORT DX11VideoMode : public VideoMode {
public:
    uint32_t numerator;
    uint32_t denominator;
    DXGI_MODE_DESC mode;

public:
    DX11VideoMode(uint32_t width, uint32_t height, float refreshRate = 60.0f, uint32_t outputIdx = 0,
                  uint32_t numerator = 60, uint32_t denominator = 1, DXGI_MODE_DESC mode = DXGI_MODE_DESC());
};

class PLUGIN_EXPORT DX11VideoOutputInfo : public VideoOutputInfo {
private:
    IDXGIOutput *mDXGIOutput;

public:
    DX11VideoOutputInfo(IDXGIOutput *output, uint32_t outputIdx);

public:
    IDXGIOutput *getDxgiOutput() const;
};

class PLUGIN_EXPORT DX11VideoModeInfo : public VideoModeInfo {
public:
    DX11VideoModeInfo(IDXGIAdapter *dxgiAdapter);
};