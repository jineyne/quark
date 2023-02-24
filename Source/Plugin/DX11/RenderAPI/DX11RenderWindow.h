#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/RenderWindow.h"
#include "Win32/Win32Window.h"

class DX11_EXPORT FDX11RenderWindow : public FRenderWindow {
private:
    FDX11Device *mDevice = nullptr;

    FWin32Window *mWindow = nullptr;

    ID3D11Texture2D *mBackBuffer = nullptr;
    ID3D11RenderTargetView *mRenderTargetView = nullptr;

    ID3D11Texture2D *mDepthStencilBuffer = nullptr;
    ID3D11DepthStencilView *mDepthStencilView = nullptr;

    IDXGIFactory1 *mDXGIFactory = nullptr;
    IDXGISwapChain *mSwapChain = nullptr;
    DXGI_SWAP_CHAIN_DESC mSwapChainDesc{};

    uint32_t mRefreshRateNumerator = 0;
    uint32_t mRefreshRateDenominator = 0;

public:
    FDX11RenderWindow(const FRenderWindowDesc &desc, uint32_t windowId, FDX11Device *device, IDXGIFactory1 *dxgiFactory, FRenderWindow *parent = nullptr);
    ~FDX11RenderWindow();

public:
    void swapBuffers(uint32_t mask) override;

    void resize(int32_t width, int32_t height) override;
    void move(int32_t left, int32_t top) override;
    void setTitle(const FString &title) override;

    bool isWindow() override { return true; }

    TArray<ID3D11RenderTargetView *> getRenderTargetView() const { return { mRenderTargetView }; }
    ID3D11DepthStencilView *getDepthStencilView() const { return mDepthStencilView; }

private:
    void createSwapChain();
    void createSizeDependedD3DResources();

    void destroySwapChain();
};