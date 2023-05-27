#pragma once

#include "DX11Prerequisites.h"

#include "RenderAPI/GpuProgram.h"

static constexpr const TCHAR *DIRECTX_COMPILER_ID = TEXT("DirectX11");

class DX11_EXPORT DX11GpuProgram : public GpuProgram {
protected:
    static uint32_t GlobalProgramId;

protected:
    uint32_t mProgramId = 0;

public:
    DX11GpuProgram(const GpuProgramDesc &desc);
    virtual ~DX11GpuProgram();

public:
    void initialize() override;

    virtual void loadFromMicrocode(DX11Device *device, const DataBlob& microcode) = 0;

    DataBlob getMicroCode() const { return mBytecode->instructions; }
    uint32_t getProgramId() const { return mProgramId; }
};

class DX11_EXPORT DX11VertexProgram : public DX11GpuProgram {
private:
    ID3D11VertexShader *mVertexShader;

public:
    DX11VertexProgram(const GpuProgramDesc &desc);

public:
    void loadFromMicrocode(DX11Device *device, const DataBlob& microcode) override;

    auto getVertexShader() const { return mVertexShader; }
};

class DX11_EXPORT DX11PixelProgram : public DX11GpuProgram {
private:
    ID3D11PixelShader *mPixelShader;

public:
    DX11PixelProgram(const GpuProgramDesc &desc);

public:
    void loadFromMicrocode(DX11Device *device, const DataBlob& microcode) override;
    auto getPixelShader() const { return mPixelShader; }
};