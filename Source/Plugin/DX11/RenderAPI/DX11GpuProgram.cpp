#include "DX11GpuProgram.h"

#include "Manager/DX11BufferManager.h"
#include "RenderAPI/DX11Device.h"
#include "RenderAPI/DX11RenderAPI.h"

uint32_t DX11GpuProgram::GlobalProgramId = 0;

DX11GpuProgram::DX11GpuProgram(const GpuProgramDesc &desc) : GpuProgram(desc) {

}

DX11GpuProgram::~DX11GpuProgram() {
    delete mInputDeclaration;
}

void DX11GpuProgram::initialize() {
    if (!isSupported()) {
        mIsCompiled = false;
        mCompileMessage = TEXT("Specified program is not supported by the current render system.");

        return;
    }

    if (!mBytecode || mBytecode->compilerId != DIRECTX_COMPILER_ID) {
        GpuProgramDesc compileDesc{};
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

        DX11RenderAPI *rapi = static_cast<DX11RenderAPI *>(RenderAPI::InstancePtr());
        loadFromMicrocode(rapi->getPrimaryDevice(), mBytecode->instructions);

        if (mType == EGpuProgramType::Vertex) {
            mInputDeclaration = BufferManager::Instance().createVertexDeclaration(mBytecode->vertexInput);
        }
    }

    mProgramId = GlobalProgramId++;
}

DX11VertexProgram::DX11VertexProgram(const GpuProgramDesc &desc) : DX11GpuProgram(desc) { }

void DX11VertexProgram::loadFromMicrocode(DX11Device *device, const DataBlob &microcode) {
    HRESULT hr = device->getDevice()->CreateVertexShader(microcode.data, microcode.size, device->getClassLinkage(),
                                                         &mVertexShader);
    if (FAILED(hr) || device->hasError()) {
        String errorMsg = device->getErrorDescription();
        EXCEPT(LogDX11, RenderAPIException, TEXT("Failed to create vertex shader from microcode\nError Description: %ls"), *errorMsg);
    }
}

DX11PixelProgram::DX11PixelProgram(const GpuProgramDesc &desc) : DX11GpuProgram(desc) { }

void DX11PixelProgram::loadFromMicrocode(DX11Device *device, const DataBlob &microcode) {
    HRESULT hr = device->getDevice()->CreatePixelShader(microcode.data, microcode.size, device->getClassLinkage(),
                                                        &mPixelShader);
    if (FAILED(hr) || device->hasError()) {
        String errorMsg = device->getErrorDescription();
        EXCEPT(LogDX11, RenderAPIException, TEXT("Failed to create pixel shader from microcode\nError Description: %ls"), *errorMsg);
    }
}
