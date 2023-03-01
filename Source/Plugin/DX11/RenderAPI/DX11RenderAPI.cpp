#include "DX11RenderAPI.h"

#include "DX11CommandBuffer.h"
#include "DX11CommandBufferManager.h"
#include "DX11Device.h"
#include "DX11Driver.h"
#include "DX11DriverList.h"
#include "DX11RenderWindow.h"
#include "DX11RenderWindowManager.h"
#include "DX11RenderStateManager.h"

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

    FRenderStateManager::StartUp<FDX11RenderStateManager>();

    mMainCommandBuffer = dynamic_cast<FDX11CommandBuffer *>(FCommandBuffer::New(EGpuQueueType::Graphics));

    D3D11_RASTERIZER_DESC rasterizerStateDesc;
    ZeroMemory(&rasterizerStateDesc, sizeof(D3D11_RASTERIZER_DESC));
    rasterizerStateDesc.AntialiasedLineEnable = false;
    rasterizerStateDesc.CullMode = D3D11_CULL_NONE;
    rasterizerStateDesc.DepthBias = 0;
    rasterizerStateDesc.DepthBiasClamp = 0.0f;
    rasterizerStateDesc.DepthClipEnable = false;
    rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerStateDesc.MultisampleEnable = false;
    rasterizerStateDesc.ScissorEnable = false;
    rasterizerStateDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerStateDesc.FrontCounterClockwise = true;
    HR(device->CreateRasterizerState(&rasterizerStateDesc, &mRasterizerState));

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = false;
    depthStencilDesc.StencilEnable = false;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    HR(device->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState));

    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0].BlendEnable = false;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    HR(device->CreateBlendState(&blendDesc, &mBlendState));

    FRenderAPI::initialize();
}

void FDX11RenderAPI::initializeWithWindow(FRenderWindow *window) {
    FRenderAPI::initializeWithWindow(window);
}

void FDX11RenderAPI::onShutDown() {
    SAFE_RELEASE(mBlendState);
    SAFE_RELEASE(mDepthStencilState);
    SAFE_RELEASE(mRasterizerState);

    FRenderStateManager::ShutDown();

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

void FDX11RenderAPI::clearRenderTarget(EFrameBufferType buffers, const FColor &color, FCommandBuffer *commandBuffer) {
    auto executeRef = [&](EFrameBufferType buffers, const FColor &color) {
        if (mActiveRenderTarget == nullptr) {
            LOG(FLogDX11, Warning, TEXT("ActiveRenderTarget not exists"));
            return;
        }

        if ((buffers & EFrameBufferType::Color) == EFrameBufferType::Color) {
            TArray<ID3D11RenderTargetView *> views;

            if (mActiveRenderTarget->isWindow()) {
                auto window = static_cast<FDX11RenderWindow *>(mActiveRenderTarget);
                views = window->getRenderTargetView();
            } else {
                //
            }

            if (views.length() <= 0) {
                return;
            }

            float clearColor[4];
            clearColor[0] = color.getRed();
            clearColor[1] = color.getGreen();
            clearColor[2] = color.getBlue();
            clearColor[3] = color.getAlpha();

            auto context = mDevice->getImmediateContext();
            for (auto view : views) {
                context->ClearRenderTargetView(view, clearColor);
            }
        }

        if ((buffers & EFrameBufferType::Depth) == EFrameBufferType::Depth || (buffers & EFrameBufferType::Stencil) == EFrameBufferType::Stencil) {
            TArray<ID3D11DepthStencilView *> views;

            if (mActiveRenderTarget->isWindow()) {
                auto window = static_cast<FDX11RenderWindow *>(mActiveRenderTarget);
                views.add(window->getDepthStencilView());
            } else {
                //
            }

            uint32_t clearFlags = 0;
            if ((buffers & EFrameBufferType::Depth) == EFrameBufferType::Depth) {
                clearFlags |= D3D11_CLEAR_DEPTH;
            }

            if ((buffers & EFrameBufferType::Stencil) == EFrameBufferType::Stencil) {
                clearFlags |= D3D11_CLEAR_STENCIL;
            }

            auto context = mDevice->getImmediateContext();
            for (auto view : views) {
                context->ClearDepthStencilView(view, clearFlags, 1.0f, 0);
            }
        }

        /*if (buffers.isSet((EFrameBufferType::Stencil))) {
            TVector<ID3D11DepthStencilView *> views;

            if (mActiveRenderTarget->isWindow()) {
                auto window = std::static_pointer_cast<DX11RenderWindow>(mActiveRenderTarget);
                views.push_back(window->getDepthStencilView());
            } else {
                //
            }

            auto context = mDevice->getImmediateContext();
            for (auto view : views) {
                context->ClearDepthStencilView(view, D3D11_CLEAR_STENCIL, 1.0f, 0);
            }
        }*/
    };
    auto execute = [=]() { executeRef(buffers, color); };

    FDX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void FDX11RenderAPI::swapBuffer(FRenderTarget *target, uint32_t mask) {
    submitCommandBuffer(mMainCommandBuffer, mask);
    target->swapBuffers();
}

void FDX11RenderAPI::submitCommandBuffer(FCommandBuffer *commandBuffer, uint32_t syncMask) {
    auto cb = getCB(commandBuffer);
    cb->executeCommands();

    if (cb == mMainCommandBuffer) {
        mMainCommandBuffer = static_cast<FDX11CommandBuffer *>(FCommandBuffer::New(EGpuQueueType::Graphics));
    }
}



FDX11CommandBuffer *FDX11RenderAPI::getCB(FCommandBuffer *buffer) {
    if (buffer != nullptr) {
        return static_cast<FDX11CommandBuffer *>(buffer);
    }

    return mMainCommandBuffer;
}



