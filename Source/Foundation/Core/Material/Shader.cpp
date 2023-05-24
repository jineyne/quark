#include "Shader.h"
#include "Exception/Exception.h"
#include "RenderAPI/GpuParams.h"

uint32_t FShader::mNextShaderId = 0;

FShaderDesc::FShaderDesc() { }

void FShaderDesc::addParameter(FShaderDataParamDesc paramDesc, uint32_t *defaultValue) {
    if (paramDesc.type == EGpuParamDataType::Struct && paramDesc.elementSize <= 0) {
        LOG(FLogMaterial, Error, TEXT("You need to provide a non-zero element size for a struct parameter."));
        return;
    }

    const auto it = dataParams.find(paramDesc.name);
    if (it != nullptr) {
        return;
    }

    if (defaultValue != nullptr) {
        paramDesc.defaultValueIdx = static_cast<uint32_t>(dataDefaultValues.length());
        auto defaultValueSize = FShader::GetDataParamSize(paramDesc.type);

        dataDefaultValues.resize(paramDesc.defaultValueIdx + defaultValueSize);
        std::memcpy(&dataDefaultValues[paramDesc.defaultValueIdx], defaultValue, defaultValueSize);
    } else {
        paramDesc.defaultValueIdx = static_cast<uint32_t>(-1);
    }

    dataParams[paramDesc.name] = paramDesc;
}

void FShaderDesc::addParameter(FShaderObjectParamDesc paramDesc) {
    auto defaultValueIdx = static_cast<uint32_t>(-1);
    addParameterInternal(std::move(paramDesc), defaultValueIdx);
}

void FShaderDesc::addParameter(FShaderObjectParamDesc paramDesc, FShaderDesc::SamplerStateType const &defaultValue) {
    auto defaultValueIdx = static_cast<uint32_t>(-1);
    if (FShader::IsSampler(paramDesc.type) && defaultValue != nullptr) {
        defaultValueIdx = static_cast<uint32_t>(samplerDefaultValues.length());
        samplerDefaultValues.add(defaultValue);
    }

    addParameterInternal(std::move(paramDesc), defaultValueIdx);
}

void FShaderDesc::addParameter(FShaderObjectParamDesc paramDesc, FShaderDesc::TextureType const &defaultValue) {
    auto defaultValueIdx = static_cast<uint32_t>(-1);
    if (FShader::IsTexture(paramDesc.type) && defaultValue != nullptr) {
        defaultValueIdx = static_cast<uint32_t>(textureDefaultValues.length());
        textureDefaultValues.add(defaultValue);
    }

    addParameterInternal(std::move(paramDesc), defaultValueIdx);
}

void FShaderDesc::addParameterAttribute(const FString &name, const FShaderParamAttribute &attrib) {
    FShaderDataParamDesc *paramDescData = nullptr;
    auto dataIt = dataParams.find(name);
    if (dataIt != nullptr) {
        paramDescData = dataIt;
    }

    FShaderObjectParamDesc *paramDescObj = nullptr;
    if (!paramDescData) {
        auto textureIt = textureParams.find(name);
        if (textureIt != nullptr) {
            paramDescObj = textureIt;
        }
    }

    if (!paramDescData) {
        auto bufferIt = bufferParams.find(name);
        if (bufferIt != nullptr) {
            paramDescObj = bufferIt;
        }
    }

    FShaderParamBase *paramDesc = paramDescData;
    if (!paramDesc) {
        paramDesc = paramDescObj;
    }

    if (!paramDesc) {
        LOG(FLogMaterial, Warning, TEXT("Attempting to apply a shader parameter attribute to a non-existing parameter."));
        return;
    }

    if (attrib.type == ShaderParamAttributeType::SpriteUV) {
        if (paramDescObj) {
            LOG(FLogMaterial, Warning, TEXT("Attempting to apply SpriteUV attribute to an object parameter is not supported."));
            return;
        }

        if (paramDescData->type != EGpuParamDataType::Float4) {
            LOG(FLogMaterial, Warning, TEXT("SpriteUV attribute can only be applied to 4D vectors."));
            return;
        }
    }

    auto curAttribIdx = paramDesc->attribIdx;
    bool found = false;
    while (curAttribIdx != static_cast<uint32_t>(-1)) {
        auto &curAttrib = paramAttributes[curAttribIdx];
        if (curAttrib.type == attrib.type) {
            curAttrib = attrib;

            found = true;
            break;
        }

        curAttribIdx = curAttrib.nextParamIdx;
    }

    if (!found) {
        const auto attribIdx = static_cast<uint32_t>(paramAttributes.length());
        paramAttributes.add(attrib);

        if (paramDesc->attribIdx != static_cast<uint32_t>(-1)) {
            paramAttributes.top().nextParamIdx = paramDesc->attribIdx;
        }

        paramDesc->attribIdx = attribIdx;
    }
}

void FShaderDesc::setParamBlockAttribs(const FString &name, bool shared, EBufferUsage usage, FString rendererSemantic) {
    FShaderParamBlockDesc desc{};
    desc.name = name;
    desc.shared = shared;
    desc.usage = usage;
    desc.rendererSemantic = rendererSemantic;

    paramBlocks[name] = desc;
}

void FShaderDesc::addParameterInternal(FShaderObjectParamDesc paramDesc, uint32_t defaultValueIndex) {
    TArray<TMap<FString, FShaderObjectParamDesc> *> destLookUp = { &textureParams, &bufferParams, &samplerParams };
    uint32_t destIdx = 0;
    if (FShader::IsBuffer(paramDesc.type)) {
        destIdx = 1;
    } else if (FShader::IsSampler(paramDesc.type)) {
        destIdx = 2;
    }

    auto &paramsMap = *destLookUp[destIdx];
    auto it = paramsMap.find(paramDesc.name);
    if (it == nullptr) {
        paramDesc.defaultValueIdx = defaultValueIndex;
        paramsMap[paramDesc.name] = paramDesc;
    } else {
        auto &desc = *it;
        if (desc.type != paramDesc.type || desc.rendererSemantic != paramDesc.rendererSemantic) {
            return;
        }

        auto &gpuVariableNames = desc.gpuVariableNames;
        bool found = false;

        for (uint32_t i = 0; i < static_cast<uint32_t>(gpuVariableNames.length()); i++) {
            if (gpuVariableNames[i] == paramDesc.gpuVariableName) {
                found = true;
                break;
            }
        }

        if (!found) {
            gpuVariableNames.add(paramDesc.gpuVariableName);
        }
    }
}

FShader::FShader(uint32_t id) : mId(id) { }

FShader::FShader(const FString &name, const FShaderDesc &desc, uint32_t id)
    :mDesc(desc), mId(id)  {
    setName(name);
}

FShader *FShader::New(const FString &name, const FShaderDesc &desc) {
    uint32_t id = FShader::mNextShaderId++;
    auto shader = new FShader(name, desc, id);
    shader->initialize();

    return shader;
}

bool FShader::IsSampler(EGpuParamObjectType type) {
    switch (type) {
        case EGpuParamObjectType::Sampler1D:
        case EGpuParamObjectType::Sampler2D:
        case EGpuParamObjectType::Sampler3D:
        case EGpuParamObjectType::SamplerCube:
        case EGpuParamObjectType::Sampler2DMS:
            return true;
        default:
            return false;
    }
}

bool FShader::IsTexture(EGpuParamObjectType type) {
    switch (type) {
        case EGpuParamObjectType::Texture1D:
        case EGpuParamObjectType::Texture2D:
        case EGpuParamObjectType::Texture3D:
        case EGpuParamObjectType::TextureCube:
        case EGpuParamObjectType::Texture2DMS:
        case EGpuParamObjectType::Texture1DArray:
        case EGpuParamObjectType::Texture2DArray:
        case EGpuParamObjectType::Texture2DMSArray:
        case EGpuParamObjectType::TextureCubeArray:
            return true;
        default:
            return false;
    }
}

bool FShader::IsBuffer(EGpuParamObjectType type) {
    switch (type) {
        case EGpuParamObjectType::ByteBuffer:
        case EGpuParamObjectType::StructuredBuffer:
        case EGpuParamObjectType::RWTypedBuffer:
            return true;
        default:
            return false;
    }
}

uint32_t FShader::GetDataParamSize(EGpuParamDataType type) {
    static const GpuDataParamInfos ParamSize;

    auto idx = static_cast<uint32_t>(type);
    if (idx < sizeof(FGpuParams::ParamSizes.lookup))
        return FGpuParams::ParamSizes.lookup[idx].size;

    return 0;
}

TArray<FShader::TechniqueType> FShader::getCompatibleTechniques() const {
    TArray<TechniqueType> output;

    for (auto &technique : mDesc.techniques) {
        if (technique->isSupported()) {
            output.add(technique);
        }
    }

    return output;
}

TArray<FShader::TechniqueType> FShader::getCompatibleTechniques(const FShaderVariation &variation, bool exact) const {
    TArray<TechniqueType> output;

    for (auto &technique : mDesc.techniques) {
        if (technique->isSupported() && technique->getVariation().matches(variation, exact)) {
            output.add(technique);
        }
    }

    return output;
}

EGpuParamType FShader::getParamType(const FString &name) const {
    auto findIterData = mDesc.dataParams.find(name);
    if (findIterData != nullptr)
        return EGpuParamType::Data;

    auto findIterTexture = mDesc.textureParams.find(name);
    if (findIterTexture != nullptr)
        return EGpuParamType::Texture;

    auto findIterBuffer = mDesc.bufferParams.find(name);
    if (findIterBuffer != nullptr)
        return EGpuParamType::Buffer;

    auto findIterSampler = mDesc.samplerParams.find(name);
    if (findIterSampler != nullptr)
        return EGpuParamType::Sampler;

    EXCEPT(FLogMaterial, InternalErrorException, TEXT("Cannot find the parameter with the name: %ls"), *name);
    return EGpuParamType::Data;
}

const FShaderDataParamDesc &FShader::getDataParamDesc(const FString &name) const {
    auto findIterData = mDesc.dataParams.find(name);
    if (findIterData != nullptr)
        return *findIterData;

    EXCEPT(FLogMaterial, InternalErrorException, TEXT("Cannot find the parameter with the name: %ls"), *name);
    static FShaderDataParamDesc dummy;
    return dummy;
}

const FShaderObjectParamDesc &FShader::getTextureParamDesc(const FString &name) const {
    auto findIterData = mDesc.textureParams.find(name);
    if (findIterData != nullptr)
        return *findIterData;

    EXCEPT(FLogMaterial, InternalErrorException, TEXT("Cannot find the parameter with the name: %ls"), *name);
    static FShaderObjectParamDesc dummy;
    return dummy;
}

const FShaderObjectParamDesc &FShader::getSamplerParamDesc(const FString &name) const {
    auto findIterData = mDesc.samplerParams.find(name);
    if (findIterData != nullptr)
        return *findIterData;

    EXCEPT(FLogMaterial, InternalErrorException, TEXT("Cannot find the parameter with the name: %ls"), *name);
    static FShaderObjectParamDesc dummy;
    return dummy;
}

const FShaderObjectParamDesc &FShader::getBufferParamDesc(const FString &name) const {
    auto findIterData = mDesc.bufferParams.find(name);
    if (findIterData != nullptr)
        return *findIterData;

    EXCEPT(FLogMaterial, InternalErrorException, TEXT("Cannot find the parameter with the name: %ls"), *name);
    static FShaderObjectParamDesc dummy;
    return dummy;
}

bool FShader::hasDataParam(const FString &name) const {
    auto it = mDesc.dataParams.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

bool FShader::hasTextureParam(const FString &name) const {
    auto it = mDesc.textureParams.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

bool FShader::hasSamplerParam(const FString &name) const {
    auto it = mDesc.samplerParams.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

bool FShader::hasBufferParam(const FString &name) const {
    auto it = mDesc.bufferParams.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

bool FShader::hasParamBlock(const FString &name) const {
    auto it = mDesc.paramBlocks.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

FShader::TextureType FShader::getDefaultTexture(uint32_t index) const {
    if (index < static_cast<uint32_t>(mDesc.textureDefaultValues.length())) {
        return mDesc.textureDefaultValues[index];
    }

    return TextureType();
}

FShader::SamplerStateType FShader::getDefaultSampler(uint32_t index) const {
    if (index < static_cast<uint32_t>(mDesc.samplerDefaultValues.length())) {
        return mDesc.samplerDefaultValues[index];
    }

    return SamplerStateType();
}

uint8_t *FShader::getDefaultValue(uint32_t index) {
    if (index < static_cast<uint32_t>(mDesc.dataDefaultValues.length())) {
        return static_cast<uint8_t *>(&mDesc.dataDefaultValues[index]);
    }

    return nullptr;
}
