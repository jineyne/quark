#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/RenderAPI.h"
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

    FRenderTarget *mActiveRenderTarget = nullptr;

public:
    void setRenderTarget(FRenderTarget *target, FCommandBuffer *commandBuffer) override;

    const FString &getName() override { return mName; }

    IDXGIFactory1 *getDXGIFactory() const { return mDXGIFactory; }
    FDX11Device *getPrimaryDevice() const { return mDevice; }

protected:
    void initialize() override;
    void initializeWithWindow(FRenderWindow *window) override;

    void onShutDown() override;

    FDX11CommandBuffer *getCB(FCommandBuffer *buffer);
};
