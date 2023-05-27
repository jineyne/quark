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
#include "Input/DX11InputManager.h"

void DX11RenderAPI::initialize() {
    RenderAPI::initialize();

    HR(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **) &mDXGIFactory));
    mDriverList = DX11DriverList::New(mDXGIFactory);
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

    mDevice = DX11Device::New(device);

    InputManager::StartUp<DX11InputManager>();

    CommandBufferManager::StartUp<DX11CommandBufferManager>();
    RenderWindowManager::StartUp<DX11RenderWindowManager>();

    RenderStateManager::StartUp<DX11RenderStateManager>();

    mMainCommandBuffer = dynamic_cast<DX11CommandBuffer *>(CommandBuffer::New(EGpuQueueType::Graphics));

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

    mIAManager = q_new<DX11InputLayoutManager>();
    RenderAPI::initialize();
}

void DX11RenderAPI::initializeWithWindow(RenderWindow *window) {
    RenderAPI::initializeWithWindow(window);

    BufferManager::StartUp<DX11BufferManager>();

    GpuProgramManager::StartUp<DX11GpuProgramManager>();
    TextureManager::StartUp<DX11TextureManager>();
}

void DX11RenderAPI::onShutDown() {
    SAFE_RELEASE(mBlendState);
    SAFE_RELEASE(mRasterizerState);

    TextureManager::ShutDown();
    GpuProgramManager::ShutDown();
    BufferManager::ShutDown();

    if (mIAManager != nullptr) {
        q_delete(mIAManager);
        mIAManager = nullptr;
    }

    RenderStateManager::ShutDown();

    RenderWindowManager::ShutDown();
    CommandBufferManager::ShutDown();

    q_delete(mDevice);
}

void DX11RenderAPI::setGraphicsPipeline(GraphicsPipelineState *pipeline, CommandBuffer *commandBuffer) {
    auto executeRef = [&](GraphicsPipelineState *pipeline) {
        DX11PixelProgram *fragmentProgram;

        if (pipeline != nullptr) {
            mActiveDepthStencilState = static_cast<DX11DepthStencilState *>(pipeline->getDepthStencilState());

            mActiveVertexShader = static_cast<DX11VertexProgram *>(pipeline->getVertexProgram());
            fragmentProgram = static_cast<DX11PixelProgram *>(pipeline->getFragmentProgram());

            if (mActiveDepthStencilState == nullptr)
                mActiveDepthStencilState = static_cast<DX11DepthStencilState *>(DepthStencilState::Default());
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

    DX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void DX11RenderAPI::setGpuParams(GpuParams *params, CommandBuffer *commandBuffer) {
    auto executeRef = [&](GpuParams *gpuParams) {
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

            GpuParamDesc *paramDesc = gpuParams->getParamDesc(type);
            if (paramDesc == nullptr) {
                return;
            }

            for (auto iter = paramDesc->textures.begin(); iter != paramDesc->textures.end(); ++iter) {
                uint32_t slot = iter->value.slot;

                FResourceHandle<Texture> texture = gpuParams->getTexture(iter->value.set, slot);
                const TextureSurface& surface = gpuParams->getTextureSurface(iter->value.set, slot);

                while (slot >= (uint32_t) srvs.length()) {
                    srvs.add(nullptr);
                }

                if (texture != nullptr) {
                    /*TextureView *texView = texture->requestView(surface.mipLevel, surface.mipLevelsCount,
                                                                 surface.face, surface.faceCount, EGpuViewUsage::Default);

                    DX11TextureView* d3d11texView = static_cast<DX11TextureView *>(texView);*/
                    srvs[slot] = static_cast<DX11Texture *>(texture.get())->getView();
                }
            }

            for (auto iter = paramDesc->buffers.begin(); iter != paramDesc->buffers.end(); ++iter) {
                uint32_t slot = iter->value.slot;
                GpuBuffer *buffer = gpuParams->getBuffer(iter->value.set, slot);

                bool isLoadStore = iter->value.type != EGpuParamObjectType::ByteBuffer &&
                                   iter->value.type != EGpuParamObjectType::StructuredBuffer;

                if (!isLoadStore) {
                    while (slot >= srvs.length()) {
                        srvs.add(nullptr);
                    }

                    if (buffer != nullptr) {
                        DX11GpuBuffer* dx11buffer = static_cast<DX11GpuBuffer*>(buffer);
                        srvs[slot] = dx11buffer->getShadeResourceView();
                    }
                } else {
                    while (slot >= uavs.length()) {
                        uavs.add(nullptr);
                    }

                    if (buffer != nullptr) {
                        DX11GpuBuffer* d3d11buffer = static_cast<DX11GpuBuffer*>(buffer);
                        uavs[slot] = d3d11buffer->getUnorderedAccessView();
                    }
                }
            }

            for (auto iter = paramDesc->samplers.begin(); iter != paramDesc->samplers.end(); ++iter) {
                uint32_t slot = iter->value.slot;
                SamplerState *samplerState = gpuParams->getSamplerState(iter->value.set, slot);

                while (slot >= (uint32_t) samplers.length())
                    samplers.add(nullptr);

                if (samplerState == nullptr)
                    samplerState = SamplerState::GetDefault();

                DX11SamplerState* sampler = static_cast<DX11SamplerState*>(const_cast<SamplerState *>(samplerState));
                samplers[slot] = sampler->getInternal();
            }

            for (auto iter = paramDesc->paramBlocks.begin(); iter != paramDesc->paramBlocks.end(); ++iter) {
                uint32_t slot = iter->value.slot;
                GpuParamBlockBuffer *buffer = gpuParams->getParamBlockBuffer(iter->value.set, slot);

                while (slot >= static_cast<uint32_t>(constBuffers.length())) {
                    constBuffers.add(nullptr);
                }

                if (buffer != nullptr) {
                    buffer->flushToGPU();

                    const DX11GpuParamBlockBuffer *d3d11paramBlockBuffer = static_cast<DX11GpuParamBlockBuffer *>(buffer);
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
            EXCEPT(LogRenderAPI, RenderAPIException, TEXT("Failed to set GPU parameters: %ls"), *mDevice->getErrorDescription());
        }
    };

    auto execute = [=]() { executeRef(params); };

    DX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void DX11RenderAPI::setVertexDeclaration(VertexDeclaration *declaration, CommandBuffer *commandBuffer) {
    auto executeRef = [&](VertexDeclaration *declaration) {
        mActiveVertexDeclaration = declaration;
    };

    auto execute = [=]() { executeRef(declaration); };

    DX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void DX11RenderAPI::setVertexBuffer(uint32_t index, const TArray<VertexBuffer *> &buffers, CommandBuffer *commandBuffer) {
    auto executeRef = [&](uint32_t index, const TArray<VertexBuffer *> &buffers) {
        TArray<ID3D11Buffer *> bounds(32);
        TArray<uint32_t> strides(32);
        TArray<uint32_t> offsets(32);

        for (auto i = 0; i < buffers.length(); i++) {
            auto buffer = static_cast<DX11VertexBuffer *>(buffers[i]);
            bounds[i] = buffer->getBuffer();

            strides[i] = buffer->getVertexSize();
            offsets[i] = 0;
        }

        auto context = mDevice->getImmediateContext();
        context->IASetVertexBuffers(0, buffers.length(), bounds.getData(), strides.getData(), offsets.getData());
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    };
    auto execute = [=]() { executeRef(index, buffers); };

    DX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void DX11RenderAPI::setViewport(const Rect &area, CommandBuffer *commandBuffer) {
    auto executeRef = [&](const Rect& vp) {
        applyViewport();
    };

    auto execute = [=]() { executeRef(area); };

    DX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}


void DX11RenderAPI::setIndexBuffer(IndexBuffer *buffer, CommandBuffer *commandBuffer) {
    auto executeRef = [&](IndexBuffer *buffer) {
        auto d3dbuffer = static_cast<DX11IndexBuffer *>(buffer);

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

    DX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void DX11RenderAPI::setRenderTarget(RenderTarget *target, CommandBuffer *commandBuffer) {
    auto executeRef = [&](RenderTarget *target) {
        mActiveRenderTarget = target;

        TArray<ID3D11RenderTargetView *> views;
        ID3D11DepthStencilView *stencilView = nullptr;

        if (target != nullptr) {
            if (target->isWindow()) {
                auto window = static_cast<DX11RenderWindow *>(target);
                views = window->getRenderTargetView();
                stencilView = window->getDepthStencilView();
            } else {
                // TODO:
            }
        }

        auto context = mDevice->getImmediateContext();
        context->OMSetRenderTargets(views.length(), views.getData(), stencilView);
        if (mDevice->hasError()) {
            EXCEPT(LogDX11, RenderAPIException, TEXT("Failed to setRenderTarget: %ls"), *mDevice->getErrorDescription());
        }
        applyViewport();
    };
    auto execute = [=]() { executeRef(target); };

    DX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void DX11RenderAPI::clearRenderTarget(EFrameBufferType buffers, const Color &color, CommandBuffer *commandBuffer) {
    auto executeRef = [&](EFrameBufferType buffers, const Color &color) {
        if (mActiveRenderTarget == nullptr) {
            LOG(LogDX11, Warning, TEXT("ActiveRenderTarget not exists"));
            return;
        }

        if ((buffers & EFrameBufferType::Color) == EFrameBufferType::Color) {
            TArray<ID3D11RenderTargetView *> views;

            if (mActiveRenderTarget->isWindow()) {
                auto window = static_cast<DX11RenderWindow *>(mActiveRenderTarget);
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
                auto window = static_cast<DX11RenderWindow *>(mActiveRenderTarget);
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
                auto window = static_cast<DX11RenderWindow*>(mActiveRenderTarget);
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

    DX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void DX11RenderAPI::swapBuffer(RenderTarget *target, uint32_t mask) {
    submitCommandBuffer(mMainCommandBuffer, mask);
    target->swapBuffers();
}

void DX11RenderAPI::draw(uint32_t vertexOffset, uint32_t vertexCount, uint32_t instanceCount,
                         CommandBuffer *commandBuffer) {
    auto executeRef = [&](uint32_t vertexOffset, uint32_t vertexCount, uint32_t instanceCount) {
        applyInputLayout();

        auto context = mDevice->getImmediateContext();

        if (instanceCount <= 1)
            context->Draw(vertexCount, vertexOffset);
        else
            context->DrawInstanced(vertexCount, instanceCount, vertexOffset, 0);

#if DEBUG_MODE
        if (mDevice->hasError()) {
            LOG(LogDX11, Warning, *mDevice->getErrorDescription());
        }
#endif
    };

    auto execute = [=]() { executeRef(vertexOffset, vertexCount, instanceCount); };

    DX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}

void DX11RenderAPI::drawIndexed(uint32_t indexOffset, uint32_t indexCount, uint32_t vertexOffset, uint32_t vertexCount,
                                uint32_t instanceCount, CommandBuffer *commandBuffer) {
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
            LOG(LogDX11, Warning, *mDevice->getErrorDescription());
        }
#endif
    };

    auto execute = [=]() { executeRef(indexOffset, indexCount, vertexOffset, vertexCount, instanceCount); };

    DX11CommandBuffer *cb = getCB(commandBuffer);
    cb->queueCommand(execute);
}


void DX11RenderAPI::submitCommandBuffer(CommandBuffer *commandBuffer, uint32_t syncMask) {
    auto cb = getCB(commandBuffer);
    cb->executeCommands();

    if (cb == mMainCommandBuffer) {
        delete mMainCommandBuffer;
        mMainCommandBuffer = static_cast<DX11CommandBuffer *>(CommandBuffer::New(EGpuQueueType::Graphics));
    }
}

GpuParamBlockDesc DX11RenderAPI::generateParamBlockDesc(const String &name, TArray<GpuParamDataDesc> &params) {
    GpuParamBlockDesc block;
    block.blockSize = 0;
    block.isShareable = true;
    block.name = name;
    block.slot = 0;
    block.set = 0;

    for (auto& param : params) {
        const GpuParamDataTypeInfo& typeInfo = GpuParams::ParamSizes.lookup[(uint32_t) param.type];

        if (param.arraySize > 1) {
            // Arrays perform no packing and their elements are always padded and aligned to four component vectors
            uint32_t size;
            if(param.type == EGpuParamDataType::Struct) {
                size = Math::DivideAndRoundUp(param.elementSize, 16U) * 4;
            } else {
                size = Math::DivideAndRoundUp(typeInfo.size, 16U) * 4;
            }

            block.blockSize = Math::DivideAndRoundUp(block.blockSize, 4U) * 4;

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
                size = Math::DivideAndRoundUp(param.elementSize, 16U) * 4;
                block.blockSize = Math::DivideAndRoundUp(block.blockSize, 4U) * 4;
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

void DX11RenderAPI::determineMultisampleSettings(uint32_t multisampleCount, DXGI_FORMAT format,
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

DX11CommandBuffer *DX11RenderAPI::getCB(CommandBuffer *buffer) {
    if (buffer != nullptr) {
        return static_cast<DX11CommandBuffer *>(buffer);
    }

    return mMainCommandBuffer;
}

void DX11RenderAPI::initCapabilities(IDXGIAdapter *adapter, RenderAPICapabilities &caps) {
    // TODO: ?
}

void DX11RenderAPI::applyViewport() {
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

void DX11RenderAPI::applyInputLayout() {
    if (mActiveVertexDeclaration == nullptr) {
        LOG(LogDX11, Warning, TEXT("Cannot apply input layout without a vertex declaration. Set vertex declaration before calling this method."));
        return;
    }

    if (mActiveVertexShader == nullptr) {
        LOG(LogRenderAPI, Warning, TEXT("Cannot apply input layout without a vertex shader. Set vertex shader before calling this method."));
        return;
    }

    DX11GpuProgram *program = static_cast<DX11GpuProgram *>(mActiveVertexShader);
    ID3D11InputLayout *ia = mIAManager->retrieveInputLayout(mActiveVertexShader->getInputDeclaration(), mActiveVertexDeclaration, program);
    mDevice->getImmediateContext()->IASetInputLayout(ia);
}
