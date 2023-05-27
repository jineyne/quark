#pragma once

#include "DX11Prerequisites.h"

#include <RenderAPI/GpuParamDesc.h>
#include <RenderAPI/GpuProgram.h>
#include <RenderAPI/VertexDeclaration.h>

class HLSLParamParser {
private:
    /** Types of HLSL parameters. */
    enum class ParamType {
        ConstantBuffer,
        Texture,
        Sampler,
        UAV,
        Count // Keep at end
    };

public:
    /** Maps a parameter in a specific shader stage, of a specific type to a unique set index. */
    static uint32_t MapParameterToSet(EGpuProgramType progType, ParamType paramType);

public:
    void parse(ID3DBlob* microcode, EGpuProgramType type, GpuParamDesc& desc, TArray<VertexElement>* inputParams);

private:
    /**
     * Parses the provided constant buffer retrieving information about all of its members and storing them in the
     * provided GPU params description object.
     */
    void parseBuffer(ID3D11ShaderReflectionConstantBuffer* bufferReflection, GpuParamDesc& desc);

    /**	Parses the resource description structure and stores it in the provided GPU params description object. */
    void parseResource(D3D11_SHADER_INPUT_BIND_DESC& resourceDesc, EGpuProgramType type, GpuParamDesc& desc);

    /**
     * Parses a variable with the specified type and variable description. Adds the variable in the provided GPU params
     * description object and assigns it to the provided param block.
     */
    void parseVariable(D3D11_SHADER_TYPE_DESC& varTypeDesc, D3D11_SHADER_VARIABLE_DESC& varDesc, GpuParamDesc& desc,
                       GpuParamBlockDesc& paramBlock);
};