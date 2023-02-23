#include "DX11RenderWindow.h"
#include "Win32/Win32Platform.h"
#include "Exception/Exception.h"

#include "DX11Device.h"

FDX11RenderWindow::FDX11RenderWindow(const FRenderWindowDesc &desc, uint32_t windowId, FDX11Device *device,
                                     IDXGIFactory1 *dxgiFactory, FRenderWindow *parent)
        : FRenderWindow(desc, windowId, parent), mDevice(device), mDXGIFactory(dxgiFactory) {
    FWin32WindowDesc windowDesc{};
    windowDesc.title = desc.title;
    windowDesc.width = desc.videoMode.width;
    windowDesc.height = desc.videoMode.height;
    windowDesc.instance = nullptr;
    windowDesc.proc = FWin32Platform::WndProc;

    mWindow = new FWin32Window(windowDesc);

    createSwapChain();
    createSizeDependedD3DResources();

    D3D11_VIEWPORT viewport;
    viewport.Width = (float) mWindow->getWidth();
    viewport.Height = (float) mWindow->getHeight();
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    mDevice->getImmediateContext()->RSSetViewports(1, &viewport);

    mDXGIFactory->MakeWindowAssociation(mWindow->getHandle(), 0);
}

FDX11RenderWindow::~FDX11RenderWindow() {
    destroySwapChain();

    delete mWindow;
}

void FDX11RenderWindow::swapBuffers(uint32_t mask) {
    mSwapChain->Present(0, 0);
}

void FDX11RenderWindow::resize(int32_t width, int32_t height) {
    mWindow->resize(width, height);
}

void FDX11RenderWindow::move(int32_t left, int32_t top) {
    mWindow->move(left, top);
}

void FDX11RenderWindow::setTitle(const FString &title) {
    mWindow->setTitle(title);
}

void FDX11RenderWindow::createSwapChain() {
    mSwapChainDesc.OutputWindow = mWindow->getHandle();
    mSwapChainDesc.BufferDesc.Width = mWidth;
    mSwapChainDesc.BufferDesc.Height = mHeight;
    mSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    mSwapChainDesc.SampleDesc.Count = 1;
    mSwapChainDesc.SampleDesc.Quality = 0;

    mSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    mSwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    mSwapChainDesc.Windowed = true;

    mSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    mSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    mSwapChainDesc.Flags = 0;

    mSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    mSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    mSwapChainDesc.BufferCount = 1;

    HRESULT hr = mDXGIFactory->CreateSwapChain(mDevice->getDevice(), &mSwapChainDesc, &mSwapChain);
    if (FAILED(hr)) {
        hr = mDXGIFactory->CreateSwapChain(mDevice->getDevice(), &mSwapChainDesc, &mSwapChain);
    }

    if (FAILED(hr)) {
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Unable to create swap chain."));
    }
}

void FDX11RenderWindow::createSizeDependedD3DResources() {
    auto device = mDevice->getDevice();
    auto context = mDevice->getImmediateContext();
    SAFE_RELEASE(mBackBuffer);
    HRESULT hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *) &mBackBuffer);

    if (FAILED(hr)) {
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Unable to get back buffer from swap chain."));
    }

    D3D11_TEXTURE2D_DESC BackBufferDesc{};
    mBackBuffer->GetDesc(&BackBufferDesc);

    D3D11_RENDER_TARGET_VIEW_DESC desc{};
    desc.Format = BackBufferDesc.Format;
    desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipSlice = 0;

    hr = device->CreateRenderTargetView(mBackBuffer, &desc, &mRenderTargetView);
    if (FAILED(hr)) {
        FString error = mDevice->getErrorDescription();
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Unable to create render target view. error: %s"), *error);
    }

    mDepthStencilView = nullptr;

    D3D11_TEXTURE2D_DESC depthBufferDesc{};
    depthBufferDesc.Width = mWidth;
    depthBufferDesc.Height = mHeight;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    hr = device->CreateTexture2D(&depthBufferDesc, nullptr, &mDepthStencilBuffer);
    if (FAILED(hr)) {
        FString error = mDevice->getErrorDescription();
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Unable to create depth stencil buffer. error: %s"), *error);
    }


    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    hr = device->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
    if (FAILED(hr)) {
        FString error = mDevice->getErrorDescription();
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Unable to create depth stencil view. error: %s"), *error);
    }
}

void FDX11RenderWindow::destroySwapChain() {
    SAFE_RELEASE(mSwapChain);
}

