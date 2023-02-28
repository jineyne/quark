#include "DX11RenderAPI.h"

#include "DX11CommandBuffer.h"
#include "DX11CommandBufferManager.h"
#include "DX11Device.h"
#include "DX11Driver.h"
#include "DX11DriverList.h"
#include "DX11RenderWindow.h"
#include "DX11RenderWindowManager.h"

void FDX11RenderAPI::initialize() {
    FRenderAPI::initialize();

    HR(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **) &mDXGIFactory));
    mDriverList = FDX11DriverList::New(mDXGIFactory);
    mActiveDriver = mDriverList->get(0);

    IDXGIAdapter *selectedAdapter = mActiveDriver->getAdapter();
    TArray<D3D_FEATURE_LEVEL> levels = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1
    };

    uint32_t deviceFlags = 0;
#if DEBUG_MODE
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    ID3D11Device *device;
    HR(D3D11CreateDevice(selectedAdapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, deviceFlags, levels.getData(),
                         levels.length(), D3D11_SDK_VERSION, &device, nullptr, nullptr));

    mDevice = FDX11Device::New(device);

    FCommandBufferManager::StartUp<FDX11CommandBufferManager>();
    FRenderWindowManager::StartUp<FDX11RenderWindowManager>();

    mMainCommandBuffer = dynamic_cast<FDX11CommandBuffer *>(FCommandBuffer::New(EGpuQueueType::Graphics));

    FRenderAPI::initialize();
}

void FDX11RenderAPI::initializeWithWindow(FRenderWindow *window) {
    FRenderAPI::initializeWithWindow(window);
}

void FDX11RenderAPI::onShutDown() {
    FRenderWindowManager::ShutDown();
    FCommandBufferManager::ShutDown();

    delete mDevice;
}

void FDX11RenderAPI::setRenderTarget(FRenderTarget *target, FCommandBuffer *commandBuffer) {
    auto executeRef = [&](FRenderTarget *target) {
        mActiveRenderTarget = target;

        TArray<ID3D11RenderTargetView *> views;
        ID3D11DepthStencilView *stencilView = nullptr;

        if (target != nullptr) {
            if (target->isWindow()) {
                auto window = static_cast<FDX11RenderWindow *>(target);
                views = window->getRenderTargetView();
                stencilView = window->getDepthStencilView();
            } else {
                // TODO:
            }
        }

        auto context = mDevice->getImmediateContext();
        context->OMSetRenderTargets(views.length(), views.getData(), stencilView);
        if (mDevice->hasError()) {
            EXCEPT(FLogDX11, RenderAPIException, TEXT("Failed to setRenderTarget: %ls"), *mDevice->getErrorDescription());
        }
        // applyViewport();
    };
    auto execute = [=]() { executeRef(target); };

    FDX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

FDX11CommandBuffer *FDX11RenderAPI::getCB(FCommandBuffer *buffer) {
    if (buffer != nullptr) {
        return static_cast<FDX11CommandBuffer *>(buffer);
    }

    return mMainCommandBuffer;
}

