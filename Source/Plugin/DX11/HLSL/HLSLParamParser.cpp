//
// Created by jiney on 2023-03-13.
//

#include "FHLSLParamParser.h"

#include <Exception/Exception.h>
#include <Math/Math.h>
#include "Utility/DX11Mapper.h"

uint32_t FHLSLParamParser::MapParameterToSet(EGpuProgramType progType, FHLSLParamParser::ParamType paramType) {
    uint32_t progTypeIdx = static_cast<uint32_t>(progType);
    uint32_t paramTypeIdx = static_cast<uint32_t>(paramType);

    return progTypeIdx * static_cast<int>(ParamType::Count) + paramTypeIdx;
}

void FHLSLParamParser::parse(ID3DBlob *microcode, EGpuProgramType type, FGpuParamDesc &desc,
                             TArray<FVertexElement> *inputParams) {
    const char *commentString = nullptr;
    ID3DBlob *pIDisassembly = nullptr;
    char *pDisassembly = nullptr;

    HRESULT hr = D3DDisassemble(static_cast<uint32_t *>(microcode->GetBufferPointer()), microcode->GetBufferSize(),
                                D3D_DISASM_ENABLE_COLOR_CODE, commentString, &pIDisassembly);
    const char *assemblyCode = static_cast<const char *>(pIDisassembly->GetBufferPointer());

    if (FAILED(hr)) {
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Unable to disassemble shader."));
    }

    ID3D11ShaderReflection *shaderReflection;
    hr = D3DReflect(static_cast<void *>(microcode->GetBufferPointer()), microcode->GetBufferSize(),
                    IID_ID3D11ShaderReflection, reinterpret_cast<void **>(&shaderReflection));

    if (FAILED(hr)) {
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Cannot reflect D3D11 high-level shader."));
    }

    D3D11_SHADER_DESC shaderDesc;
    hr = shaderReflection->GetDesc(&shaderDesc);

    if (FAILED(hr)) {
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Cannot reflect D3D11 high-level shader."));
    }

    if (inputParams != nullptr) {
        D3D11_SIGNATURE_PARAMETER_DESC inputParamDesc;
        for (uint32_t i = 0; i < shaderDesc.InputParameters; i++) {
            hr = shaderReflection->GetInputParameterDesc(i, &inputParamDesc);

            if (FAILED(hr)) {
                EXCEPT(FLogDX11, RenderAPIException, TEXT("Cannot get input param desc with index: %ld"), i);
            }

            // We don't care about system value semantics
            if (FString(inputParamDesc.SemanticName).startWith(TEXT("sv_"))) {
                continue;
            }

            inputParams->add(FVertexElement(inputParamDesc.Stream, inputParamDesc.Register,
                                            FDX11Mapper::GetInputType(inputParamDesc.ComponentType),
                                            FDX11Mapper::Get(inputParamDesc.SemanticName),
                                            inputParamDesc.SemanticIndex));
        }
    }

    for (uint32_t i = 0; i < shaderDesc.BoundResources; i++) {
        D3D11_SHADER_INPUT_BIND_DESC bindingDesc;
        hr = shaderReflection->GetResourceBindingDesc(i, &bindingDesc);

        if (FAILED(hr)) {
            EXCEPT(FLogDX11, RenderAPIException, TEXT("Cannot get resource binding desc with index: %ld"), i);
        }

        parseResource(bindingDesc, type, desc);
    }

    for (uint32_t i = 0; i < shaderDesc.ConstantBuffers; i++) {
        ID3D11ShaderReflectionConstantBuffer *shaderReflectionConstantBuffer;
        shaderReflectionConstantBuffer = shaderReflection->GetConstantBufferByIndex(i);

        parseBuffer(shaderReflectionConstantBuffer, desc);
    }

    shaderReflection->Release();
}

void FHLSLParamParser::parseBuffer(ID3D11ShaderReflectionConstantBuffer *bufferReflection, FGpuParamDesc &desc) {
    D3D11_SHADER_BUFFER_DESC constantBufferDesc;
    HRESULT hr = bufferReflection->GetDesc(&constantBufferDesc);
    if (FAILED(hr)) {
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Failed to retrieve HLSL constant buffer description."));
    }

    if (constantBufferDesc.Type != D3D_CT_CBUFFER && constantBufferDesc.Type != D3D_CT_TBUFFER) {
        // Not supported (most likely a constant buffer used internally by DX)
        return;
    }

    FGpuParamBlockDesc &blockDesc = desc.paramBlocks[constantBufferDesc.Name];

    for (uint32_t j = 0; j < constantBufferDesc.Variables; j++) {
        ID3D11ShaderReflectionVariable *varRef = bufferReflection->GetVariableByIndex(j);
        D3D11_SHADER_VARIABLE_DESC varDesc;
        hr = varRef->GetDesc(&varDesc);

        if (FAILED(hr)) {
            EXCEPT(FLogDX11, RenderAPIException, TEXT("Failed to retrieve HLSL constant buffer variable description."));
        }

        ID3D11ShaderReflectionType *varRefType = varRef->GetType();
        D3D11_SHADER_TYPE_DESC varTypeDesc;
        varRefType->GetDesc(&varTypeDesc);

        parseVariable(varTypeDesc, varDesc, desc, blockDesc);
    }

    blockDesc.blockSize = constantBufferDesc.Size / 4;
}

void FHLSLParamParser::parseResource(D3D11_SHADER_INPUT_BIND_DESC &resourceDesc, EGpuProgramType type,
                                     FGpuParamDesc &desc) {
    for (uint32_t i = 0; i < resourceDesc.BindCount; i++) {
        if (resourceDesc.Type == D3D_SIT_CBUFFER || resourceDesc.Type == D3D_SIT_TBUFFER) {
            FGpuParamBlockDesc blockDesc;
            blockDesc.name = resourceDesc.Name;
            blockDesc.slot = resourceDesc.BindPoint + i;
            blockDesc.set = MapParameterToSet(type, ParamType::ConstantBuffer);
            blockDesc.blockSize = 0; // Calculated manually as we add parameters

            if (strcmp(resourceDesc.Name, "$Globals") == 0 || strcmp(resourceDesc.Name, "$Param") == 0)  {
                // Special buffers, as defined by DX11 docs
                blockDesc.isShareable = false;
            } else {
                blockDesc.isShareable = true;
            }

            desc.paramBlocks.add(blockDesc.name, blockDesc);
        } else {
            FGpuParamObjectDesc memberDesc;
            memberDesc.name = resourceDesc.Name;
            memberDesc.slot = resourceDesc.BindPoint + i;
            memberDesc.type = EGpuParamObjectType::Unknown;

            switch (resourceDesc.Type) {
                case D3D_SIT_SAMPLER:
                    memberDesc.type = EGpuParamObjectType::Sampler2D; // Actual dimension of the sampler doesn't matter
                    memberDesc.set = MapParameterToSet(type, ParamType::Sampler);

                    desc.samplers.add(memberDesc.name, memberDesc);
                    break;
                case D3D_SIT_TEXTURE: {
                    bool isTexture = true;
                    switch (resourceDesc.Dimension) {
                        case D3D_SRV_DIMENSION_TEXTURE1D:
                            memberDesc.type = EGpuParamObjectType::Texture1D;
                            break;
                        case D3D_SRV_DIMENSION_TEXTURE1DARRAY:
                            memberDesc.type = EGpuParamObjectType::Texture1DArray;
                            break;
                        case D3D_SRV_DIMENSION_TEXTURE2D:
                            memberDesc.type = EGpuParamObjectType::Texture2D;
                            break;
                        case D3D_SRV_DIMENSION_TEXTURE2DARRAY:
                            memberDesc.type = EGpuParamObjectType::Texture2DArray;
                            break;
                        case D3D_SRV_DIMENSION_TEXTURE3D:
                            memberDesc.type = EGpuParamObjectType::Texture3D;
                            break;
                        case D3D_SRV_DIMENSION_TEXTURECUBE:
                            memberDesc.type = EGpuParamObjectType::TextureCube;
                            break;
                        case D3D_SRV_DIMENSION_TEXTURECUBEARRAY:
                            memberDesc.type = EGpuParamObjectType::TextureCubeArray;
                            break;
                        case D3D_SRV_DIMENSION_TEXTURE2DMS:
                            memberDesc.type = EGpuParamObjectType::Texture2DMS;
                            break;
                        case D3D_SRV_DIMENSION_TEXTURE2DMSARRAY:
                            memberDesc.type = EGpuParamObjectType::Texture2DMSArray;
                            break;
                        case D3D_SRV_DIMENSION_BUFFER:
                            memberDesc.type = EGpuParamObjectType::ByteBuffer;
                            isTexture = false;
                            break;
                        default:
                            LOG(FLogDX11, Warning, TEXT("Skipping texture because it has unsupported dimension: %ld"), resourceDesc.Dimension);
                    }

                    if (memberDesc.type != EGpuParamObjectType::Unknown) {
                        memberDesc.set = MapParameterToSet(type, ParamType::Texture);

                        if (isTexture) {
                            desc.textures.add(memberDesc.name, memberDesc);
                        } else {
                            desc.buffers.add(memberDesc.name, memberDesc);
                        }
                    }
                }
                    break;
                case D3D_SIT_STRUCTURED:
                    memberDesc.type = EGpuParamObjectType::StructuredBuffer;
                    memberDesc.set = MapParameterToSet(type, ParamType::Texture);

                    desc.buffers.add(memberDesc.name, memberDesc);
                    break;
                case D3D_SIT_BYTEADDRESS:
                    memberDesc.type = EGpuParamObjectType::ByteBuffer;
                    memberDesc.set = MapParameterToSet(type, ParamType::Texture);

                    desc.buffers.add(memberDesc.name, memberDesc);
                    break;
                case D3D11_SIT_UAV_RWTYPED: {
                    memberDesc.set = MapParameterToSet(type, ParamType::UAV);

                    switch (resourceDesc.Dimension) {
                        /*case D3D_SRV_DIMENSION_TEXTURE1D:
                            memberDesc.type = GpuParamObjectType::RWTEXTURE1D;
                            desc.loadStoreTextures.add(memberDesc.name, memberDesc);
                            break;
                        case D3D_SRV_DIMENSION_TEXTURE1DARRAY:
                            memberDesc.type = GpuParamObjectType::;
                            desc.loadStoreTextures.add(memberDesc.name, memberDesc);
                            break;
                        case D3D_SRV_DIMENSION_TEXTURE2D:
                            memberDesc.type = GpuParamObjectType::;
                            desc.loadStoreTextures.add(memberDesc.name, memberDesc);
                            break;
                        case D3D_SRV_DIMENSION_TEXTURE2DARRAY:
                            memberDesc.type = GpuParamObjectType::;
                            desc.loadStoreTextures.add(memberDesc.name, memberDesc);
                            break;
                        case D3D_SRV_DIMENSION_TEXTURE3D:
                            memberDesc.type = GpuParamObjectType::;
                            desc.loadStoreTextures.add(memberDesc.name, memberDesc);
                            break;
                        case D3D_SRV_DIMENSION_TEXTURE2DMS:
                            memberDesc.type = GpuParamObjectType::;
                            desc.loadStoreTextures.add(memberDesc.name, memberDesc);
                            break;
                        case D3D_SRV_DIMENSION_TEXTURE2DMSARRAY:
                            memberDesc.type = GpuParamObjectType::;
                            desc.loadStoreTextures.add(memberDesc.name, memberDesc);
                            break;*/
                        case D3D_SRV_DIMENSION_BUFFER:
                            memberDesc.type = EGpuParamObjectType::RWTypedBuffer;
                            desc.buffers.add(memberDesc.name, memberDesc);
                            break;
                        default:
                            LOG(FLogDX11, Warning, TEXT("Skipping typed UAV because it has unsupported dimension: %ld"), resourceDesc.Dimension);
                    }

                    break;
                }
                    /*case D3D11_SIT_UAV_RWSTRUCTURED:
                        memberDesc.type = GpuParamObjectType::;
                        memberDesc.set = mapParameterToSet(type, EParamType::UAV);

                        desc.buffers.add(memberDesc.name, memberDesc));
                        break;
                    case D3D11_SIT_UAV_RWBYTEADDRESS:
                        memberDesc.type = GpuParamObjectType::;
                        memberDesc.set = mapParameterToSet(type, EParamType::UAV);

                        desc.buffers.add(memberDesc.name, memberDesc));
                        break;
                    case D3D_SIT_UAV_APPEND_STRUCTURED:
                        memberDesc.type = GpuParamObjectType::;
                        memberDesc.set = mapParameterToSet(type, EParamType::UAV);

                        desc.buffers.add(memberDesc.name, memberDesc));
                        break;
                    case D3D_SIT_UAV_CONSUME_STRUCTURED:
                        memberDesc.type = GpuParamObjectType::;
                        memberDesc.set = mapParameterToSet(type, EParamType::UAV);

                        desc.buffers.add(memberDesc.name, memberDesc));
                        break;
                    case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:
                        memberDesc.type = GpuParamObjectType::;
                        memberDesc.set = mapParameterToSet(type, EParamType::UAV);

                        desc.buffers.add(memberDesc.name, memberDesc));
                        break;*/
                default:
                    LOG(FLogDX11, Warning, TEXT("Skipping resource because it has unsupported type: %ld"), resourceDesc.Type);
            }
        }
    }
}

void FHLSLParamParser::parseVariable(D3D11_SHADER_TYPE_DESC &varTypeDesc, D3D11_SHADER_VARIABLE_DESC &varDesc,
                                     FGpuParamDesc &desc, FGpuParamBlockDesc &paramBlock) {
    FGpuParamDataDesc memberDesc;
    memberDesc.name = varDesc.Name;
    memberDesc.paramBlockSlot = paramBlock.slot;
    memberDesc.paramBlockSet = paramBlock.set;
    memberDesc.arraySize = varTypeDesc.Elements == 0 ? 1 : varTypeDesc.Elements;
    memberDesc.gpuMemOffset = varDesc.StartOffset / 4;
    memberDesc.cpuMemOffset = varDesc.StartOffset / 4;

    // Determine individual element size in the array
    if (memberDesc.arraySize > 1) {
        // Find array element size (reported size is total size of array, minus unused register slots)
        int totalArraySize = (varDesc.Size / 4);

        int totalSlotsUsedByArray = FMath::DivideAndRoundUp(totalArraySize, 4) * 4;
        int unusedSlotsInArray = totalSlotsUsedByArray - totalArraySize;

        memberDesc.arrayElementStride = totalSlotsUsedByArray / memberDesc.arraySize;
        memberDesc.elementSize = memberDesc.arrayElementStride - unusedSlotsInArray;
    } else {
        memberDesc.elementSize = varDesc.Size / 4; // Stored in multiples of 4
        memberDesc.arrayElementStride = memberDesc.elementSize;
    }

    switch (varTypeDesc.Class) {
        case D3D_SVC_SCALAR: {
            switch (varTypeDesc.Type) {
                case D3D_SVT_BOOL:
                    memberDesc.type = EGpuParamDataType::Bool;
                    break;
                case D3D_SVT_INT:
                case D3D_SVT_UINT:
                    memberDesc.type = EGpuParamDataType::Int1;
                    break;
                case D3D_SVT_FLOAT:
                    memberDesc.type = EGpuParamDataType::Float1;
                    break;
                default:
                    LOG(FLogDX11, Warning, TEXT("Skipping variable because it has unsupported type: %ld"), varTypeDesc.Type);
            }
        }
            break;
        case D3D_SVC_VECTOR: {
            switch (varTypeDesc.Type) {
                case D3D_SVT_UINT:
                case D3D_SVT_INT: {
                    switch (varTypeDesc.Columns) {
                        case 1:
                            memberDesc.type = EGpuParamDataType::Int1;
                            break;
                        case 2:
                            memberDesc.type = EGpuParamDataType::Int2;
                            break;
                        case 3:
                            memberDesc.type = EGpuParamDataType::Int3;
                            break;
                        case 4:
                            memberDesc.type = EGpuParamDataType::Int4;
                            break;
                    }
                }

                    break;
                case D3D_SVT_FLOAT: {
                    switch (varTypeDesc.Columns) {
                        case 1:
                            memberDesc.type = EGpuParamDataType::Float1;
                            break;
                        case 2:
                            memberDesc.type = EGpuParamDataType::Float2;
                            break;
                        case 3:
                            memberDesc.type = EGpuParamDataType::Float3;
                            break;
                        case 4:
                            memberDesc.type = EGpuParamDataType::Float4;
                            break;
                    }
                }

                    break;
            }
        }
            break;
        case D3D_SVC_MATRIX_COLUMNS:
        case D3D_SVC_MATRIX_ROWS:
            switch (varTypeDesc.Rows) {
                case 2:
                    switch (varTypeDesc.Columns) {
                        case 2:
                            memberDesc.type = EGpuParamDataType::Matrix2x2;
                            break;
                        case 3:
                            memberDesc.type = EGpuParamDataType::Matrix2x3;
                            break;
                        case 4:
                            memberDesc.type = EGpuParamDataType::Matrix2x4;
                            break;
                    }
                    break;
                case 3:
                    switch (varTypeDesc.Columns) {
                        case 2:
                            memberDesc.type = EGpuParamDataType::Matrix3x2;
                            break;
                        case 3:
                            memberDesc.type = EGpuParamDataType::Matrix3x3;
                            break;
                        case 4:
                            memberDesc.type = EGpuParamDataType::Matrix3x4;
                            break;
                    }
                    break;
                case 4:
                    switch (varTypeDesc.Columns) {
                        case 2:
                            memberDesc.type = EGpuParamDataType::Matrix4x2;
                            break;
                        case 3:
                            memberDesc.type = EGpuParamDataType::Matrix4x3;
                            break;
                        case 4:
                            memberDesc.type = EGpuParamDataType::Matrix4x4;
                            break;
                    }
                    break;
            }
            break;
        case D3D_SVC_STRUCT:
            memberDesc.type = EGpuParamDataType::Struct;
            break;
        default:
            LOG(FLogDX11, Warning, TEXT("Skipping variable because it has unsupported class: %ld"), varTypeDesc.Class);
    }

    desc.params.add(memberDesc.name, memberDesc);
}