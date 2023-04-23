#include "DX11RenderAPI.h"

#include "DX11CommandBuffer.h"
#include "Manager/DX11BufferManager.h"
#include "Manager/DX11CommandBufferManager.h"
#include "Manager/DX11GpuProgramManager.h"
#include "Manager/DX11InputLayoutManager.h"
#include "Manager/DX11RenderWindowManager.h"
#include "Manager/DX11RenderStateManager.h"
#include "Manager/DX11TextureManager.h"
#include "RenderAPI/GraphicsPipelineState.h"
#include "RenderAPI/GpuParams.h"
#include "DX11Device.h"
#include "DX11Driver.h"
#include "DX11DriverList.h"
#include "DX11GpuProgram.h"
#include "DX11RenderWindow.h"
#include "DX11IndexBuffer.h"
#include "DX11VertexBuffer.h"
#include "DX11GpuParamBlockBuffer.h"
#include "Image/DX11Texture.h"
#include "DX11TextureView.h"
#include "DX11SamplerState.h"
#include "DX11GpuBuffer.h"

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

    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0].BlendEnable = false;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    HR(device->CreateBlendState(&blendDesc, &mBlendState));

    mIAManager = q_new<FDX11InputLayoutManager>();
    FRenderAPI::initialize();
}

void FDX11RenderAPI::initializeWithWindow(FRenderWindow *window) {
    FRenderAPI::initializeWithWindow(window);

    FBufferManager::StartUp<FDX11BufferManager>();

    FGpuProgramManager::StartUp<FDX11GpuProgramManager>();
    FTextureManager::StartUp<FDX11TextureManager>();
}

void FDX11RenderAPI::onShutDown() {
    SAFE_RELEASE(mBlendState);
    SAFE_RELEASE(mRasterizerState);

    FTextureManager::ShutDown();
    FGpuProgramManager::ShutDown();
    FBufferManager::ShutDown();

    if (mIAManager != nullptr) {
        q_delete(mIAManager);
        mIAManager = nullptr;
    }

    FRenderStateManager::ShutDown();

    FRenderWindowManager::ShutDown();
    FCommandBufferManager::ShutDown();

    q_delete(mDevice);
}

void FDX11RenderAPI::setGraphicsPipeline(FGraphicsPipelineState *pipeline, FCommandBuffer *commandBuffer) {
    auto executeRef = [&](FGraphicsPipelineState *pipeline) {
        FDX11PixelProgram *fragmentProgram;

        if (pipeline != nullptr) {
            mActiveDepthStencilState = static_cast<FDX11DepthStencilState *>(pipeline->getDepthStencilState());

            mActiveVertexShader = static_cast<FDX11VertexProgram *>(pipeline->getVertexProgram());
            fragmentProgram = static_cast<FDX11PixelProgram *>(pipeline->getFragmentProgram());

            if (mActiveDepthStencilState == nullptr)
                mActiveDepthStencilState = static_cast<FDX11DepthStencilState *>(FDepthStencilState::Default());
        } else {
            mActiveVertexShader = nullptr;
            fragmentProgram = nullptr;

            mActiveDepthStencilState = nullptr;
        }

        ID3D11DeviceContext *context = mDevice->getImmediateContext();
        context->RSSetState(mRasterizerState);
        context->OMSetDepthStencilState(mActiveDepthStencilState->getInternal(), 1);

        float blendFactor[4] = { 0, 0, 0, 0 };
        context->OMSetBlendState(mBlendState, blendFactor, 0xffffffff);

        if (mActiveVertexShader != nullptr) {
            context->VSSetShader(mActiveVertexShader->getVertexShader(), nullptr, 0);
        } else {
            context->VSSetShader(nullptr, nullptr, 0);
        }

        if (fragmentProgram != nullptr) {
            context->PSSetShader(fragmentProgram->getPixelShader(), nullptr, 0);
        } else {
            context->PSSetShader(nullptr, nullptr, 0);
        }
    };
    auto execute = [=]() { executeRef(pipeline); };

    FDX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void FDX11RenderAPI::setGpuParams(FGpuParams *params, FCommandBuffer *commandBuffer) {
    auto executeRef = [&](FGpuParams *gpuParams) {
        ID3D11DeviceContext *context = mDevice->getImmediateContext();

        TArray<ID3D11ShaderResourceView *> srvs(8);
        TArray<ID3D11UnorderedAccessView *> uavs(8);
        TArray<ID3D11Buffer *> constBuffers(8);
        TArray<ID3D11SamplerState *> samplers(8);

        auto populateViews = [&](EGpuProgramType type) {
            srvs.clear();
            uavs.clear();
            constBuffers.clear();
            samplers.clear();

            FGpuParamDesc *paramDesc = gpuParams->getParamDesc(type);
            if (paramDesc == nullptr) {
                return;
            }

            for (auto iter = paramDesc->textures.begin(); iter != paramDesc->textures.end(); ++iter) {
                uint32_t slot = iter->second.slot;

                FResourceHandle<FTexture> texture = gpuParams->getTexture(iter->second.set, slot);
                const FTextureSurface& surface = gpuParams->getTextureSurface(iter->second.set, slot);

                while (slot >= (uint32_t) srvs.length()) {
                    srvs.add(nullptr);
                }

                if (texture != nullptr) {
                    /*FTextureView *texView = texture->requestView(surface.mipLevel, surface.mipLevelsCount,
                                                                 surface.face, surface.faceCount, EGpuViewUsage::Default);

                    FDX11TextureView* d3d11texView = static_cast<FDX11TextureView *>(texView);*/
                    srvs[slot] = static_cast<FDX11Texture *>(texture.get())->getView();
                }
            }

            for (auto iter = paramDesc->buffers.begin(); iter != paramDesc->buffers.end(); ++iter) {
                uint32_t slot = iter->second.slot;
                FGpuBuffer *buffer = gpuParams->getBuffer(iter->second.set, slot);

                bool isLoadStore = iter->second.type != EGpuParamObjectType::ByteBuffer &&
                                   iter->second.type != EGpuParamObjectType::StructuredBuffer;

                if (!isLoadStore) {
                    while (slot >= srvs.length()) {
                        srvs.add(nullptr);
                    }

                    if (buffer != nullptr) {
                        FDX11GpuBuffer* dx11buffer = static_cast<FDX11GpuBuffer*>(buffer);
                        srvs[slot] = dx11buffer->getShadeResourceView();
                    }
                } else {
                    while (slot >= uavs.length()) {
                        uavs.add(nullptr);
                    }

                    if (buffer != nullptr) {
                        FDX11GpuBuffer* d3d11buffer = static_cast<FDX11GpuBuffer*>(buffer);
                        uavs[slot] = d3d11buffer->getUnorderedAccessView();
                    }
                }
            }

            for (auto iter = paramDesc->samplers.begin(); iter != paramDesc->samplers.end(); ++iter) {
                uint32_t slot = iter->second.slot;
                FSamplerState *samplerState = gpuParams->getSamplerState(iter->second.set, slot);

                while (slot >= (uint32_t) samplers.length())
                    samplers.add(nullptr);

                if (samplerState == nullptr)
                    samplerState = FSamplerState::GetDefault();

                FDX11SamplerState* sampler = static_cast<FDX11SamplerState*>(const_cast<FSamplerState *>(samplerState));
                samplers[slot] = sampler->getInternal();
            }

            for (auto iter = paramDesc->paramBlocks.begin(); iter != paramDesc->paramBlocks.end(); ++iter) {
                uint32_t slot = iter->second.slot;
                FGpuParamBlockBuffer *buffer = gpuParams->getParamBlockBuffer(iter->second.set, slot);

                while (slot >= static_cast<uint32_t>(constBuffers.length())) {
                    constBuffers.add(nullptr);
                }

                if (buffer != nullptr) {
                    buffer->flushToGPU();

                    const FDX11GpuParamBlockBuffer *d3d11paramBlockBuffer = static_cast<FDX11GpuParamBlockBuffer *>(buffer);
                    constBuffers[slot] = d3d11paramBlockBuffer->getBuffer();
                }
            }
        };

        uint32_t numSRVs = 0;
        uint32_t numUAVs = 0;
        uint32_t numConstBuffers = 0;
        uint32_t numSamplers = 0;

        populateViews(EGpuProgramType::Vertex);
        numSRVs = static_cast<uint32_t>(srvs.length());
        numConstBuffers = static_cast<uint32_t>(constBuffers.length());
        numSamplers = static_cast<uint32_t>(samplers.length());

        if (numSRVs > 0) {
            context->VSSetShaderResources(0, numSRVs, *srvs);
        }

        if (numConstBuffers > 0) {
            context->VSSetConstantBuffers(0, numConstBuffers, *constBuffers);
        }

        if (numSamplers > 0) {
            context->VSSetSamplers(0, numSamplers, *samplers);
        }

        populateViews(EGpuProgramType::Fragment);
        numSRVs = (uint32_t) srvs.length();
        numUAVs = (uint32_t) uavs.length();
        numConstBuffers = (uint32_t) constBuffers.length();
        numSamplers = (uint32_t) samplers.length();

        if (numSRVs > 0)
            context->PSSetShaderResources(0, numSRVs, *srvs);

        if (numUAVs > 0) {
            context->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL, nullptr,
                                                               nullptr, 0, numUAVs, *uavs, nullptr);
        }

        if (numConstBuffers > 0) {
            context->PSSetConstantBuffers(0, numConstBuffers, *constBuffers);
        }

        if (numSamplers > 0) {
            context->PSSetSamplers(0, numSamplers, *samplers);
        }

        if (numConstBuffers > 0) {
            context->CSSetConstantBuffers(0, numConstBuffers, *constBuffers);
        }

        if (numSamplers > 0) {
            context->CSSetSamplers(0, numSamplers, *samplers);
        }

        if (mDevice->hasError()) {
            EXCEPT(FLogRenderAPI, RenderAPIException, TEXT("Failed to set GPU parameters: %ls"), *mDevice->getErrorDescription());
        }
    };

    auto execute = [=]() { executeRef(params); };

    FDX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void FDX11RenderAPI::setVertexDeclaration(FVertexDeclaration *declaration, FCommandBuffer *commandBuffer) {
    auto executeRef = [&](FVertexDeclaration *declaration) {
        mActiveVertexDeclaration = declaration;
    };

    auto execute = [=]() { executeRef(declaration); };

    FDX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void FDX11RenderAPI::setVertexBuffer(uint32_t index, const TArray<FVertexBuffer *> &buffers, FCommandBuffer *commandBuffer) {
    auto executeRef = [&](uint32_t index, const TArray<FVertexBuffer *> &buffers) {
        TArray<ID3D11Buffer *> bounds(32);
        TArray<uint32_t> strides(32);
        TArray<uint32_t> offsets(32);

        for (auto i = 0; i < buffers.length(); i++) {
            auto buffer = static_cast<FDX11VertexBuffer *>(buffers[i]);
            bounds[i] = buffer->getBuffer();

            strides[i] = buffer->getVertexSize();
            offsets[i] = 0;
        }

        auto context = mDevice->getImmediateContext();
        context->IASetVertexBuffers(0, buffers.length(), bounds.getData(), strides.getData(), offsets.getData());
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    };
    auto execute = [=]() { executeRef(index, buffers); };

    FDX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void FDX11RenderAPI::setViewport(const FRect &area, FCommandBuffer *commandBuffer) {
    auto executeRef = [&](const FRect& vp) {
        applyViewport();
    };

    auto execute = [=]() { executeRef(area); };

    FDX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}


void FDX11RenderAPI::setIndexBuffer(FIndexBuffer *buffer, FCommandBuffer *commandBuffer) {
    auto executeRef = [&](FIndexBuffer *buffer) {
        auto d3dbuffer = static_cast<FDX11IndexBuffer *>(buffer);

        DXGI_FORMAT format = DXGI_FORMAT_R16_UINT;
        switch (buffer->getIndexType()) {
            case EIndexType::_16bit:
                format = DXGI_FORMAT_R16_UINT;
                break;

            case EIndexType::_32bit:
                format = DXGI_FORMAT_R32_UINT;
                break;
        }

        auto context = mDevice->getImmediateContext();
        context->IASetIndexBuffer(d3dbuffer->getBuffer(), format, 0);
    };
    auto execute = [=]() { executeRef(buffer); };

    FDX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
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
        applyViewport();
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

        /*if ((buffers & EFrameBufferType::Stencil) == EFrameBufferType::Stencil) {
            TArray<ID3D11DepthStencilView *> views;

            if (mActiveRenderTarget->isWindow()) {
                auto window = static_cast<FDX11RenderWindow*>(mActiveRenderTarget);
                views.add(window->getDepthStencilView());
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

void FDX11RenderAPI::draw(uint32_t vertexOffset, uint32_t vertexCount, uint32_t instanceCount,
                          FCommandBuffer *commandBuffer) {
    auto executeRef = [&](uint32_t vertexOffset, uint32_t vertexCount, uint32_t instanceCount) {
        applyInputLayout();

        auto context = mDevice->getImmediateContext();

        if (instanceCount <= 1)
            context->Draw(vertexCount, vertexOffset);
        else
            context->DrawInstanced(vertexCount, instanceCount, vertexOffset, 0);

#if DEBUG_MODE
        if (mDevice->hasError()) {
            LOG(FLogDX11, Warning, *mDevice->getErrorDescription());
        }
#endif
    };

    auto execute = [=]() { executeRef(vertexOffset, vertexCount, instanceCount); };

    FDX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void FDX11RenderAPI::drawIndexed(uint32_t indexOffset, uint32_t indexCount, uint32_t vertexOffset, uint32_t vertexCount,
                                 uint32_t instanceCount, FCommandBuffer *commandBuffer) {
    auto executeRef = [&](uint32_t startIndex, uint32_t indexCount, uint32_t vertexOffset,
                          uint32_t vertexCount, uint32_t instanceCount) {
        applyInputLayout();

        auto context = mDevice->getImmediateContext();

        if (instanceCount <= 1)
            context->DrawIndexed(indexCount, startIndex, vertexOffset);
        else
            context->DrawIndexedInstanced(indexCount, instanceCount, startIndex, vertexOffset, 0);

#if DEBUG_MODE
        if (mDevice->hasError()) {
            LOG(FLogDX11, Warning, *mDevice->getErrorDescription());
        }
#endif
    };

    auto execute = [=]() { executeRef(indexOffset, indexCount, vertexOffset, vertexCount, instanceCount); };

    FDX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}


void FDX11RenderAPI::submitCommandBuffer(FCommandBuffer *commandBuffer, uint32_t syncMask) {
    auto cb = getCB(commandBuffer);
    cb->executeCommands();

    if (cb == mMainCommandBuffer) {
        delete mMainCommandBuffer;
        mMainCommandBuffer = static_cast<FDX11CommandBuffer *>(FCommandBuffer::New(EGpuQueueType::Graphics));
    }
}

FGpuParamBlockDesc FDX11RenderAPI::generateParamBlockDesc(const FString &name, TArray<FGpuParamDataDesc> &params) {
    FGpuParamBlockDesc block;
    block.blockSize = 0;
    block.isShareable = true;
    block.name = name;
    block.slot = 0;
    block.set = 0;

    for (auto& param : params) {
        const GpuParamDataTypeInfo& typeInfo = FGpuParams::ParamSizes.lookup[(uint32_t) param.type];

        if (param.arraySize > 1) {
            // Arrays perform no packing and their elements are always padded and aligned to four component vectors
            uint32_t size;
            if(param.type == EGpuParamDataType::Struct) {
                size = FMath::DivideAndRoundUp(param.elementSize, 16U) * 4;
            } else {
                size = FMath::DivideAndRoundUp(typeInfo.size, 16U) * 4;
            }

            block.blockSize = FMath::DivideAndRoundUp(block.blockSize, 4U) * 4;

            param.elementSize = size;
            param.arrayElementStride = size;
            param.cpuMemOffset = block.blockSize;
            param.gpuMemOffset = 0;

            // Last array element isn't rounded up to four component vectors unless it's a struct
            if(param.type != EGpuParamDataType::Struct) {
                block.blockSize += size * (param.arraySize - 1);
                block.blockSize += typeInfo.size / 4;
            } else {
                block.blockSize += param.arraySize * size;
            }
        } else {
            uint32_t size;
            if(param.type == EGpuParamDataType::Struct)
            {
                // Structs are always aligned and arounded up to 4 component vectors
                size = FMath::DivideAndRoundUp(param.elementSize, 16U) * 4;
                block.blockSize = FMath::DivideAndRoundUp(block.blockSize, 4U) * 4;
            }
            else
            {
                size = typeInfo.baseTypeSize * (typeInfo.numRows * typeInfo.numColumns) / 4;

                // Pack everything as tightly as possible as long as the data doesn't cross 16 byte boundary
                UINT32 alignOffset = block.blockSize % 4;
                if (alignOffset != 0 && size > (4 - alignOffset))
                {
                    UINT32 padding = (4 - alignOffset);
                    block.blockSize += padding;
                }
            }

            param.elementSize = size;
            param.arrayElementStride = size;
            param.cpuMemOffset = block.blockSize;
            param.gpuMemOffset = 0;

            block.blockSize += size;
        }

        param.paramBlockSlot = 0;
        param.paramBlockSet = 0;
    }

    // Constant buffer size must always be a multiple of 16
    if (block.blockSize % 4 != 0) {
        block.blockSize += (4 - (block.blockSize % 4));
    }

    return block;
}

void FDX11RenderAPI::determineMultisampleSettings(uint32_t multisampleCount, DXGI_FORMAT format,
                                                  DXGI_SAMPLE_DESC *outputSampleDesc) {
    if (multisampleCount == 0 || multisampleCount == 1) {
        outputSampleDesc->Count = 1;
        outputSampleDesc->Quality = 0;

        return;
    }


    // NO CSAA NOW
    outputSampleDesc->Count = multisampleCount == 0 ? 1 : multisampleCount;
    outputSampleDesc->Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;
}

FDX11CommandBuffer *FDX11RenderAPI::getCB(FCommandBuffer *buffer) {
    if (buffer != nullptr) {
        return static_cast<FDX11CommandBuffer *>(buffer);
    }

    return mMainCommandBuffer;
}

void FDX11RenderAPI::initCapabilities(IDXGIAdapter *adapter, FRenderAPICapabilities &caps) {
    // TODO: ?
}

void FDX11RenderAPI::applyViewport() {
    if (mActiveRenderTarget == nullptr)
        return;

    // Set viewport dimensions
    mViewport.TopLeftX = (float) (mActiveRenderTarget->getWidth() * mViewportNorm.x);
    mViewport.TopLeftY = (float) (mActiveRenderTarget->getHeight() * mViewportNorm.y);
    mViewport.Width = (float) (mActiveRenderTarget->getWidth() * mViewportNorm.width);
    mViewport.Height = (float) (mActiveRenderTarget->getHeight() * mViewportNorm.height);

    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;

    mDevice->getImmediateContext()->RSSetViewports(1, &mViewport);
}

void FDX11RenderAPI::applyInputLayout() {
    if (mActiveVertexDeclaration == nullptr) {
        LOG(FLogDX11, Warning, TEXT("Cannot apply input layout without a vertex declaration. Set vertex declaration before calling this method."));
        return;
    }

    if (mActiveVertexShader == nullptr) {
        LOG(FLogRenderAPI, Warning, TEXT("Cannot apply input layout without a vertex shader. Set vertex shader before calling this method."));
        return;
    }

    FDX11GpuProgram *program = static_cast<FDX11GpuProgram *>(mActiveVertexShader);
    ID3D11InputLayout *ia = mIAManager->retrieveInputLayout(mActiveVertexShader->getInputDeclaration(), mActiveVertexDeclaration, program);
    mDevice->getImmediateContext()->IASetInputLayout(ia);
}
