#include "DX11GpuProgram.h"

#include "Manager/DX11BufferManager.h"
#include "RenderAPI/DX11Device.h"
#include "RenderAPI/DX11RenderAPI.h"

uint32_t FDX11GpuProgram::GlobalProgramId = 0;

FDX11GpuProgram::FDX11GpuProgram(const FGpuProgramDesc &desc) : FGpuProgram(desc) {

}

FDX11GpuProgram::~FDX11GpuProgram() {
    delete mInputDeclaration;
}

void FDX11GpuProgram::initialize() {
    if (!isSupported()) {
        mIsCompiled = false;
        mCompileMessage = TEXT("Specified program is not supported by the current render system.");

        return;
    }

    if (!mBytecode || mBytecode->compilerId != DIRECTX_COMPILER_ID) {
        FGpuProgramDesc compileDesc{};
        compileDesc.type = mType;
        compileDesc.entryPoint = mEntryPoint;
        compileDesc.source = mSource;
        compileDesc.language = "hlsl";

        mBytecode = CompileBytecode(compileDesc);
    }

    mCompileMessage = mBytecode->messages;
    mIsCompiled = mBytecode->instructions.data != nullptr;

    if (mIsCompiled) {
        mParametersDesc = mBytecode->paramDesc;

        FDX11RenderAPI *rapi = static_cast<FDX11RenderAPI *>(FRenderAPI::InstancePtr());
        loadFromMicrocode(rapi->getPrimaryDevice(), mBytecode->instructions);

        if (mType == EGpuProgramType::Vertex) {
            mInputDeclaration = FBufferManager::Instance().createVertexDeclaration(mBytecode->vertexInput);
        }
    }

    mProgramId = GlobalProgramId++;
}

FDX11VertexProgram::FDX11VertexProgram(const FGpuProgramDesc &desc) : FDX11GpuProgram(desc) { }

void FDX11VertexProgram::loadFromMicrocode(FDX11Device *device, const FDataBlob &microcode) {
    HRESULT hr = device->getDevice()->CreateVertexShader(microcode.data, microcode.size, device->getClassLinkage(),
                                                         &mVertexShader);
    if (FAILED(hr) || device->hasError()) {
        FString errorMsg = device->getErrorDescription();
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Failed to create vertex shader from microcode\nError Description: %ls"), *errorMsg);
    }
}

FDX11PixelProgram::FDX11PixelProgram(const FGpuProgramDesc &desc) : FDX11GpuProgram(desc) { }

void FDX11PixelProgram::loadFromMicrocode(FDX11Device *device, const FDataBlob &microcode) {
    HRESULT hr = device->getDevice()->CreatePixelShader(microcode.data, microcode.size, device->getClassLinkage(),
                                                        &mPixelShader);
    if (FAILED(hr) || device->hasError()) {
        FString errorMsg = device->getErrorDescription();
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Failed to create pixel shader from microcode\nError Description: %ls"), *errorMsg);
    }
}
