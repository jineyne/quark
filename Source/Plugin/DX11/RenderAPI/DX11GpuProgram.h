#pragma once

#include "DX11Prerequisites.h"

#include "RenderAPI/GpuProgram.h"

static constexpr const TCHAR *DIRECTX_COMPILER_ID = TEXT("DirectX11");

class DX11_EXPORT FDX11GpuProgram : public FGpuProgram {
protected:
    static uint32_t GlobalProgramId;

protected:
    uint32_t mProgramId = 0;

public:
    FDX11GpuProgram(const FGpuProgramDesc &desc);
    virtual ~FDX11GpuProgram();

public:
    void initialize() override;

    virtual void loadFromMicrocode(FDX11Device *device, const FDataBlob& microcode) = 0;

    FDataBlob getMicroCode() const { return mBytecode->instructions; }
    uint32_t getProgramId() const { return mProgramId; }
};

class DX11_EXPORT FDX11VertexProgram : public FDX11GpuProgram {
private:
    ID3D11VertexShader *mVertexShader;

public:
    FDX11VertexProgram(const FGpuProgramDesc &desc);

public:
    void loadFromMicrocode(FDX11Device *device, const FDataBlob& microcode) override;

    auto getVertexShader() const { return mVertexShader; }
};

class DX11_EXPORT FDX11PixelProgram : public FDX11GpuProgram {
private:
    ID3D11PixelShader *mPixelShader;

public:
    FDX11PixelProgram(const FGpuProgramDesc &desc);

public:
    void loadFromMicrocode(FDX11Device *device, const FDataBlob& microcode) override;
    auto getPixelShader() const { return mPixelShader; }
};