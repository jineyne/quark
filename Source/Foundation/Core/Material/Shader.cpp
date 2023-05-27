#include "Shader.h"
#include "Exception/Exception.h"
#include "RenderAPI/GpuParams.h"

uint32_t Shader::mNextShaderId = 0;

ShaderParamBase::ShaderParamBase(const String &name, const String &gpuVariableName, String rendererSemantic)
        : name(name), gpuVariableName(gpuVariableName), rendererSemantic(rendererSemantic) {}

ShaderDataParamDesc::ShaderDataParamDesc(const String &name, const String &gpuVariableName, EGpuParamDataType type,
                                         uint32_t arraySize, uint32_t elementSize)
        : ShaderParamBase(name, gpuVariableName), type(type), arraySize(arraySize), elementSize(elementSize) {}

ShaderObjectParamDesc::ShaderObjectParamDesc(const String &name, const String &gpuVariableName,
                                             EGpuParamObjectType type)
        : ShaderParamBase(name, gpuVariableName), type(type) {
    gpuVariableNames.add(gpuVariableName);
}


ShaderDesc::ShaderDesc() { }

void ShaderDesc::addParameter(ShaderDataParamDesc paramDesc, uint32_t *defaultValue) {
    if (paramDesc.type == EGpuParamDataType::Struct && paramDesc.elementSize <= 0) {
        LOG(LogMaterial, Error, TEXT("You need to provide a non-zero element size for a struct parameter."));
        return;
    }

    const auto it = dataParams.find(paramDesc.name);
    if (it != nullptr) {
        return;
    }

    if (defaultValue != nullptr) {
        paramDesc.defaultValueIdx = static_cast<uint32_t>(dataDefaultValues.length());
        auto defaultValueSize = Shader::GetDataParamSize(paramDesc.type);

        dataDefaultValues.resize(paramDesc.defaultValueIdx + defaultValueSize);
        std::memcpy(&dataDefaultValues[paramDesc.defaultValueIdx], defaultValue, defaultValueSize);
    } else {
        paramDesc.defaultValueIdx = static_cast<uint32_t>(-1);
    }

    dataParams[paramDesc.name] = paramDesc;
}

void ShaderDesc::addParameter(ShaderObjectParamDesc paramDesc) {
    auto defaultValueIdx = static_cast<uint32_t>(-1);
    addParameterInternal(std::move(paramDesc), defaultValueIdx);
}

void ShaderDesc::addParameter(ShaderObjectParamDesc paramDesc, ShaderDesc::SamplerStateType const &defaultValue) {
    auto defaultValueIdx = static_cast<uint32_t>(-1);
    if (Shader::IsSampler(paramDesc.type) && defaultValue != nullptr) {
        defaultValueIdx = static_cast<uint32_t>(samplerDefaultValues.length());
        samplerDefaultValues.add(defaultValue);
    }

    addParameterInternal(std::move(paramDesc), defaultValueIdx);
}

void ShaderDesc::addParameter(ShaderObjectParamDesc paramDesc, ShaderDesc::TextureType const &defaultValue) {
    auto defaultValueIdx = static_cast<uint32_t>(-1);
    if (Shader::IsTexture(paramDesc.type) && defaultValue != nullptr) {
        defaultValueIdx = static_cast<uint32_t>(textureDefaultValues.length());
        textureDefaultValues.add(defaultValue);
    }

    addParameterInternal(std::move(paramDesc), defaultValueIdx);
}

void ShaderDesc::addParameterAttribute(const String &name, const ShaderParamAttribute &attrib) {
    ShaderDataParamDesc *paramDescData = nullptr;
    auto dataIt = dataParams.find(name);
    if (dataIt != nullptr) {
        paramDescData = dataIt;
    }

    ShaderObjectParamDesc *paramDescObj = nullptr;
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

    ShaderParamBase *paramDesc = paramDescData;
    if (!paramDesc) {
        paramDesc = paramDescObj;
    }

    if (!paramDesc) {
        LOG(LogMaterial, Warning, TEXT("Attempting to apply a shader parameter attribute to a non-existing parameter."));
        return;
    }

    if (attrib.type == EShaderParamAttributeType::SpriteUV) {
        if (paramDescObj) {
            LOG(LogMaterial, Warning, TEXT("Attempting to apply SpriteUV attribute to an object parameter is not supported."));
            return;
        }

        if (paramDescData->type != EGpuParamDataType::Float4) {
            LOG(LogMaterial, Warning, TEXT("SpriteUV attribute can only be applied to 4D vectors."));
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

void ShaderDesc::setParamBlockAttribs(const String &name, bool shared, EBufferUsage usage, String rendererSemantic) {
    ShaderParamBlockDesc desc{};
    desc.name = name;
    desc.shared = shared;
    desc.usage = usage;
    desc.rendererSemantic = rendererSemantic;

    paramBlocks[name] = desc;
}

void ShaderDesc::addParameterInternal(ShaderObjectParamDesc paramDesc, uint32_t defaultValueIndex) {
    TArray<TMap<String, ShaderObjectParamDesc> *> destLookUp = {&textureParams, &bufferParams, &samplerParams };
    uint32_t destIdx = 0;
    if (Shader::IsBuffer(paramDesc.type)) {
        destIdx = 1;
    } else if (Shader::IsSampler(paramDesc.type)) {
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

Shader::Shader(uint32_t id) : mId(id) { }

Shader::Shader(const String &name, const ShaderDesc &desc, uint32_t id)
    :mDesc(desc), mId(id)  {
    setName(name);
}

Shader *Shader::New(const String &name, const ShaderDesc &desc) {
    uint32_t id = Shader::mNextShaderId++;
    auto shader = new Shader(name, desc, id);
    shader->initialize();

    return shader;
}

bool Shader::IsSampler(EGpuParamObjectType type) {
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

bool Shader::IsTexture(EGpuParamObjectType type) {
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

bool Shader::IsBuffer(EGpuParamObjectType type) {
    switch (type) {
        case EGpuParamObjectType::ByteBuffer:
        case EGpuParamObjectType::StructuredBuffer:
        case EGpuParamObjectType::RWTypedBuffer:
            return true;
        default:
            return false;
    }
}

uint32_t Shader::GetDataParamSize(EGpuParamDataType type) {
    static const GpuDataParamInfos ParamSize;

    auto idx = static_cast<uint32_t>(type);
    if (idx < sizeof(GpuParams::ParamSizes.lookup))
        return GpuParams::ParamSizes.lookup[idx].size;

    return 0;
}

TArray<Shader::TechniqueType> Shader::getCompatibleTechniques() const {
    TArray<TechniqueType> output;

    for (auto &technique : mDesc.techniques) {
        if (technique->isSupported()) {
            output.add(technique);
        }
    }

    return output;
}

TArray<Shader::TechniqueType> Shader::getCompatibleTechniques(const FShaderVariation &variation, bool exact) const {
    TArray<TechniqueType> output;

    for (auto &technique : mDesc.techniques) {
        if (technique->isSupported() && technique->getVariation().matches(variation, exact)) {
            output.add(technique);
        }
    }

    return output;
}

EGpuParamType Shader::getParamType(const String &name) const {
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

    EXCEPT(LogMaterial, InternalErrorException, TEXT("Cannot find the parameter with the name: %ls"), *name);
    return EGpuParamType::Data;
}

const ShaderDataParamDesc &Shader::getDataParamDesc(const String &name) const {
    auto findIterData = mDesc.dataParams.find(name);
    if (findIterData != nullptr)
        return *findIterData;

    EXCEPT(LogMaterial, InternalErrorException, TEXT("Cannot find the parameter with the name: %ls"), *name);
    static ShaderDataParamDesc dummy;
    return dummy;
}

const ShaderObjectParamDesc &Shader::getTextureParamDesc(const String &name) const {
    auto findIterData = mDesc.textureParams.find(name);
    if (findIterData != nullptr)
        return *findIterData;

    EXCEPT(LogMaterial, InternalErrorException, TEXT("Cannot find the parameter with the name: %ls"), *name);
    static ShaderObjectParamDesc dummy;
    return dummy;
}

const ShaderObjectParamDesc &Shader::getSamplerParamDesc(const String &name) const {
    auto findIterData = mDesc.samplerParams.find(name);
    if (findIterData != nullptr)
        return *findIterData;

    EXCEPT(LogMaterial, InternalErrorException, TEXT("Cannot find the parameter with the name: %ls"), *name);
    static ShaderObjectParamDesc dummy;
    return dummy;
}

const ShaderObjectParamDesc &Shader::getBufferParamDesc(const String &name) const {
    auto findIterData = mDesc.bufferParams.find(name);
    if (findIterData != nullptr)
        return *findIterData;

    EXCEPT(LogMaterial, InternalErrorException, TEXT("Cannot find the parameter with the name: %ls"), *name);
    static ShaderObjectParamDesc dummy;
    return dummy;
}

bool Shader::hasDataParam(const String &name) const {
    auto it = mDesc.dataParams.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

bool Shader::hasTextureParam(const String &name) const {
    auto it = mDesc.textureParams.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

bool Shader::hasSamplerParam(const String &name) const {
    auto it = mDesc.samplerParams.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

bool Shader::hasBufferParam(const String &name) const {
    auto it = mDesc.bufferParams.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

bool Shader::hasParamBlock(const String &name) const {
    auto it = mDesc.paramBlocks.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

Shader::TextureType Shader::getDefaultTexture(uint32_t index) const {
    if (index < static_cast<uint32_t>(mDesc.textureDefaultValues.length())) {
        return mDesc.textureDefaultValues[index];
    }

    return TextureType();
}

Shader::SamplerStateType Shader::getDefaultSampler(uint32_t index) const {
    if (index < static_cast<uint32_t>(mDesc.samplerDefaultValues.length())) {
        return mDesc.samplerDefaultValues[index];
    }

    return SamplerStateType();
}

uint8_t *Shader::getDefaultValue(uint32_t index) {
    if (index < static_cast<uint32_t>(mDesc.dataDefaultValues.length())) {
        return static_cast<uint8_t *>(&mDesc.dataDefaultValues[index]);
    }

    return nullptr;
}
