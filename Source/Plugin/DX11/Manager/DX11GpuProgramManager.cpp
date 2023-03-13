#include "DX11GpuProgramManager.h"
#include "RenderAPI/DX11GpuProgram.h"
#include "HLSL/FHLSLParamParser.h"

#include <regex>

uint32_t parseErrorMessage(const char* message) {
    if (message == nullptr) {
        return 0;
    }

    std::string pattern = R"(\(([0-9]*),.*\))";
    std::regex regex(pattern);

    std::cmatch results;
    if (std::regex_search(message, results, regex)) {
        std::string result = results[1].str();
        return strtol(result.c_str(), nullptr, 10) - 1;
    }

    return 0;
}

FGpuProgramBytecode *FDX11GpuProgramManager::compileBytecode(const FGpuProgramDesc &desc) {
    std::string hlslProfile;
    switch (desc.type) {
        case EGpuProgramType::Vertex:
            hlslProfile = "vs_5_0";
            break;
        case EGpuProgramType::Fragment:
            hlslProfile = "ps_5_0";
            break;
        case EGpuProgramType::Geometry:
            hlslProfile = "gs_5_0";
            break;
        case EGpuProgramType::Compute:
            hlslProfile = "cs_5_0";
            break;
        case EGpuProgramType::Hull:
            hlslProfile = "hs_5_0";
            break;
        case EGpuProgramType::Domain:
            hlslProfile = "ds_5_0";
            break;
        default:
            break;
    }

    uint32_t compileFlags = 0;
#if defined(DEBUG_MODE)
    compileFlags |= D3DCOMPILE_DEBUG;
    compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    compileFlags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

    ID3DBlob *microcode = nullptr;
    ID3DBlob *messages = nullptr;

    const std::string &source = TCHAR_TO_ANSI(*desc.source);
    const std::string &entryPoint = TCHAR_TO_ANSI(*desc.entryPoint);

    const D3D_SHADER_MACRO defines[] = {
            {"HLSL",          "1"},
            {"MARU_Z_OFFSET", "1"},
            {nullptr,         nullptr}
    };

    HRESULT hr = D3DCompile(source.c_str(), source.size(), nullptr, defines, nullptr,
                            entryPoint.c_str(), hlslProfile.c_str(), compileFlags, 0, &microcode, &messages);
    FString compileMessage;
    if (messages != nullptr) {
        const char* message = static_cast<const char*>(messages->GetBufferPointer());
        UINT32 lineIdx = parseErrorMessage(message);

        auto sourceLines = desc.source.split("\n");
        FString sourceLine;
        if (lineIdx < sourceLines.length())
            sourceLine = sourceLines[lineIdx];

        compileMessage = FString::Printf(TEXT("%ls\n\nLine %d: %d"), ANSI_TO_TCHAR(message), lineIdx, *sourceLine);
        SAFE_RELEASE(messages);
    }

    FGpuProgramBytecode *bytecode = new FGpuProgramBytecode();
    bytecode->compilerId = DIRECTX_COMPILER_ID;
    bytecode->messages = compileMessage;

    if (FAILED(hr)) {
        SAFE_RELEASE(microcode);
        return bytecode;
    }

    if (microcode != nullptr) {
        bytecode->instructions.size = static_cast<uint32_t>(microcode->GetBufferSize());
        bytecode->instructions.data = static_cast<uint8_t *>(malloc(bytecode->instructions.size));

        memcpy(bytecode->instructions.data, microcode->GetBufferPointer(), bytecode->instructions.size);

        // TOO: GENERATE PARAM PARSER
        FHLSLParamParser parser;
        bytecode->paramDesc = new FGpuParamDesc();

        if (desc.type == EGpuProgramType::Vertex) {
            parser.parse(microcode, desc.type, *bytecode->paramDesc, &bytecode->vertexInput);
        } else {
            parser.parse(microcode, desc.type, *bytecode->paramDesc, nullptr);
        }
    }

    SAFE_RELEASE(microcode);
    return bytecode;
}

FGpuProgram *FDX11GpuProgramManager::createInternal(const FGpuProgramDesc &desc) {
    switch (desc.type) {
        case EGpuProgramType::Vertex:
            return new FDX11VertexProgram(desc);

        case EGpuProgramType::Fragment:
            return new FDX11PixelProgram(desc);

        default:
            EXCEPT(FLogDX11, RenderAPIException, TEXT("Given shader type is not support"));
            return nullptr;
    }
}
