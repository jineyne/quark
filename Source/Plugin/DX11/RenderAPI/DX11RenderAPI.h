#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/DX11GpuProgram.h"
#include "RenderAPI/RenderAPI.h"
#include "DX11DepthStencilState.h"
#include "DX11RenderAPI.g.h"

QCLASS()
class DX11_EXPORT FDX11RenderAPI : public FRenderAPI {
    GENERATED_BODY()

private:
    FString mName = TEXT("quark-dx11");

    IDXGIFactory1 *mDXGIFactory = nullptr;
    FDX11Device *mDevice = nullptr;
    FDX11DriverList *mDriverList = nullptr;
    FDX11Driver *mActiveDriver = nullptr;

    FDX11VertexProgram *mActiveVertexShader = nullptr;
    FVertexDeclaration *mActiveVertexDeclaration = nullptr;
    FDX11DepthStencilState *mActiveDepthStencilState = nullptr;
    FRenderTarget *mActiveRenderTarget = nullptr;

    FRect mViewportNorm = FRect(0.0f, 0.0f, 1.0f, 1.0f);
    D3D11_VIEWPORT mViewport;

    ID3D11BlendState *mBlendState = nullptr;
    ID3D11RasterizerState *mRasterizerState = nullptr;

    FDX11InputLayoutManager *mIAManager = nullptr;

    FDX11CommandBuffer *mMainCommandBuffer = nullptr;
    bool mDrawCallInProgress = false;
    uint32_t mStencilRef = 0;

public:
    void setGraphicsPipeline(FGraphicsPipelineState *pipelineState, FCommandBuffer *commandBuffer) override;
    void setGpuParams(FGpuParams *params, FCommandBuffer *commandBuffer) override;
    void setVertexDeclaration(FVertexDeclaration *declaration, FCommandBuffer *commandBuffer) override;
    void setVertexBuffer(uint32_t index, const TArray<FVertexBuffer *> &buffers, FCommandBuffer *commandBuffer = nullptr) override;
    void setViewport(const FRect &area, FCommandBuffer *commandBuffer) override;
    void setIndexBuffer(FIndexBuffer *buffer, FCommandBuffer *commandBuffer = nullptr) override;
    void setRenderTarget(FRenderTarget *target, FCommandBuffer *commandBuffer) override;

    void clearRenderTarget(EFrameBufferType buffers, const FColor &color, FCommandBuffer *commandBuffer) override;
    void swapBuffer(FRenderTarget *target, uint32_t mask) override;
    void draw(uint32_t vertexOffset, uint32_t vertexCount, uint32_t instanceCount, FCommandBuffer *commandBuffer) override;
    void drawIndexed(uint32_t indexOffset, uint32_t indexCount, uint32_t vertexOffset, uint32_t vertexCount,
                     uint32_t instanceCount, FCommandBuffer *commandBuffer) override;

    void submitCommandBuffer(FCommandBuffer *commandBuffer, uint32_t syncMask) override;
    FGpuParamBlockDesc generateParamBlockDesc(const FString &name, TArray<FGpuParamDataDesc> &params) override;

    void determineMultisampleSettings(uint32_t multisampleCount, DXGI_FORMAT format, DXGI_SAMPLE_DESC *outputSampleDesc);

    const FString &getName() override { return mName; }

    IDXGIFactory1 *getDXGIFactory() const { return mDXGIFactory; }
    FDX11Device *getPrimaryDevice() const { return mDevice; }


protected:
    void initialize() override;
    void initializeWithWindow(FRenderWindow *window) override;

    void onShutDown() override;

private:
    void initCapabilities(IDXGIAdapter* adapter, FRenderAPICapabilities& caps);

    void applyViewport();
    void applyInputLayout();

    FDX11CommandBuffer *getCB(FCommandBuffer *buffer);
};
