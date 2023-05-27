#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/DX11GpuProgram.h"
#include "RenderAPI/RenderAPI.h"
#include "DX11DepthStencilState.h"
#include "DX11RenderAPI.g.h"

QCLASS()
class DX11_EXPORT DX11RenderAPI : public RenderAPI {
    GENERATED_BODY()

private:
    String mName = TEXT("quark-dx11");

    IDXGIFactory1 *mDXGIFactory = nullptr;
    DX11Device *mDevice = nullptr;
    DX11DriverList *mDriverList = nullptr;
    DX11Driver *mActiveDriver = nullptr;

    DX11VertexProgram *mActiveVertexShader = nullptr;
    VertexDeclaration *mActiveVertexDeclaration = nullptr;
    DX11DepthStencilState *mActiveDepthStencilState = nullptr;
    RenderTarget *mActiveRenderTarget = nullptr;

    Rect mViewportNorm = Rect(0.0f, 0.0f, 1.0f, 1.0f);
    D3D11_VIEWPORT mViewport;

    ID3D11BlendState *mBlendState = nullptr;
    ID3D11RasterizerState *mRasterizerState = nullptr;

    DX11InputLayoutManager *mIAManager = nullptr;

    DX11CommandBuffer *mMainCommandBuffer = nullptr;
    bool mDrawCallInProgress = false;
    uint32_t mStencilRef = 0;

public:
    void setGraphicsPipeline(GraphicsPipelineState *pipelineState, CommandBuffer *commandBuffer) override;
    void setGpuParams(GpuParams *params, CommandBuffer *commandBuffer) override;
    void setVertexDeclaration(VertexDeclaration *declaration, CommandBuffer *commandBuffer) override;
    void setVertexBuffer(uint32_t index, const TArray<VertexBuffer *> &buffers, CommandBuffer *commandBuffer = nullptr) override;
    void setViewport(const Rect &area, CommandBuffer *commandBuffer) override;
    void setIndexBuffer(IndexBuffer *buffer, CommandBuffer *commandBuffer = nullptr) override;
    void setRenderTarget(RenderTarget *target, CommandBuffer *commandBuffer) override;

    void clearRenderTarget(EFrameBufferType buffers, const Color &color, CommandBuffer *commandBuffer) override;
    void swapBuffer(RenderTarget *target, uint32_t mask) override;
    void draw(uint32_t vertexOffset, uint32_t vertexCount, uint32_t instanceCount, CommandBuffer *commandBuffer) override;
    void drawIndexed(uint32_t indexOffset, uint32_t indexCount, uint32_t vertexOffset, uint32_t vertexCount,
                     uint32_t instanceCount, CommandBuffer *commandBuffer) override;

    void submitCommandBuffer(CommandBuffer *commandBuffer, uint32_t syncMask) override;
    GpuParamBlockDesc generateParamBlockDesc(const String &name, TArray<GpuParamDataDesc> &params) override;

    void determineMultisampleSettings(uint32_t multisampleCount, DXGI_FORMAT format, DXGI_SAMPLE_DESC *outputSampleDesc);

    const String &getName() override { return mName; }

    IDXGIFactory1 *getDXGIFactory() const { return mDXGIFactory; }
    DX11Device *getPrimaryDevice() const { return mDevice; }


protected:
    void initialize() override;
    void initializeWithWindow(RenderWindow *window) override;

    void onShutDown() override;

private:
    void initCapabilities(IDXGIAdapter* adapter, RenderAPICapabilities& caps);

    void applyViewport();
    void applyInputLayout();

    DX11CommandBuffer *getCB(CommandBuffer *buffer);
};
