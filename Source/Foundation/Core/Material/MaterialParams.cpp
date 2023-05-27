#include "MaterialParams.h"

#include "Material/Shader.h"
#include "Image/Texture.h"
#include "RenderAPI/GpuBuffer.h"
#include "RenderAPI/SamplerState.h"

MaterialParamsBase::MaterialParamsBase(const TMap<String, ShaderDataParamDesc> &dataParams,
                                       const TMap<String, ShaderObjectParamDesc> &textureParams,
                                       const TMap<String, ShaderObjectParamDesc> &bufferParams,
                                       const TMap<String, ShaderObjectParamDesc> &samplerParams,
                                       uint64_t initialParamVersion)
        : mParamVersion(initialParamVersion) {
    mDataSize = 0;
    for (auto &param: dataParams) {
        if (param.value.type == EGpuParamDataType::Unknown) {
            continue;
        }

        uint32_t arraySize = param.value.arraySize > 1 ? param.value.arraySize : 1;
        if (param.value.type == EGpuParamDataType::Struct) {
            mStructParamsCount += arraySize;
        } else {
            const auto &typeInfo = GpuParams::ParamSizes.lookup[static_cast<uint32_t>(param.value.type)];
            uint32_t paramSize = typeInfo.numColumns * typeInfo.numRows * typeInfo.baseTypeSize;

            mDataSize += arraySize * paramSize;
            mDataParamsCount += arraySize;
        }
    }

    mTextureParamsCount = static_cast<uint32_t>(textureParams.length());
    mBufferParamsCount = static_cast<uint32_t>(bufferParams.length());
    mSamplerParamsCount = static_cast<uint32_t>(samplerParams.length());

    mDataParamsBuffer.resize(mDataSize);
    std::fill(mDataParamsBuffer.begin(), mDataParamsBuffer.end(), 0);

    mDataParams.resize(mDataParamsCount);
    if (mDataParamsCount > 0) {
        std::memset(&mDataParams[0], 0, sizeof(DataParamInfo) * mDataParams.length());
    }

    uint32_t dataParamIdx = 0;
    uint32_t dataBufferIdx = 0;
    uint32_t structParamIdx = 0;

    for (auto &entry: dataParams) {
        if (entry.value.type == EGpuParamDataType::Unknown) {
            continue;
        }

        const auto paramIdx = static_cast<uint32_t>(mParams.length());
        mParams.add(ParamData());
        mParamLookup[entry.key] = paramIdx;

        auto &dataParam = mParams.top();

        const uint32_t arraySize = entry.value.arraySize > 1 ? entry.value.arraySize : 1;
        dataParam.arraySize = arraySize;
        dataParam.type = EParamType::Data;
        dataParam.dataType = entry.value.type;
        dataParam.version = 1;

        if (entry.value.type == EGpuParamDataType::Struct) {
            dataParam.index = structParamIdx;
            structParamIdx += arraySize;
        } else {
            dataParam.index = dataParamIdx;

            const GpuParamDataTypeInfo &typeInfo = GpuParams::ParamSizes.lookup[(int) dataParam.dataType];
            const uint32_t paramSize = typeInfo.numColumns * typeInfo.numRows * typeInfo.baseTypeSize;
            for (uint32_t i = 0; i < arraySize; i++) {
                mDataParams[dataParamIdx].offset = dataBufferIdx;
                mDataParams[dataParamIdx].spriteTextureIdx = static_cast<uint32_t>(-1);

                dataBufferIdx += paramSize;
                dataParamIdx++;
            }
        }
    }

    uint32_t textureIdx = 0;
    for (auto &entry : textureParams) {
        auto paramIdx = static_cast<uint32_t>(mParams.length());
        mParams.add(ParamData());
        mParamLookup[entry.key] = paramIdx;

        auto &dataParam = mParams.top();
        dataParam.arraySize = 1;
        dataParam.type = EParamType::Texture;
        dataParam.dataType = EGpuParamDataType::Unknown;
        dataParam.index = textureIdx;
        dataParam.version = 1;

        textureIdx++;
    }

    uint32_t bufferIdx = 0;
    for (auto &entry: bufferParams) {
        auto paramIdx = static_cast<uint32_t>(mParams.length());
        mParams.add(ParamData());
        mParamLookup[entry.key] = paramIdx;

        auto &dataParam = mParams.top();
        dataParam.arraySize = 1;
        dataParam.type = EParamType::Buffer;
        dataParam.dataType = EGpuParamDataType::Unknown;
        dataParam.index = bufferIdx;
        dataParam.version = 1;

        bufferIdx++;
    }

    uint32_t samplerIdx = 0;
    for (auto &entry: samplerParams) {
        auto paramIdx = static_cast<uint32_t>(mParams.length());
        mParams.add(ParamData());
        mParamLookup[entry.key] = paramIdx;

        auto &dataParam = mParams.top();
        dataParam.arraySize = 1;
        dataParam.type = EParamType::Sampler;
        dataParam.dataType = EGpuParamDataType::Unknown;
        dataParam.index = samplerIdx;
        dataParam.version = 1;

        samplerIdx++;
    }
}

MaterialParamsBase::~MaterialParamsBase() {
        mDataParamsBuffer.clear();
        mDataParams.clear();
}

uint32_t MaterialParamsBase::getParamIndex(const String &name) const {
    auto it = mParamLookup.find(name);
    if (it == nullptr) {
        return static_cast<uint32_t>(-1);
    }

    return *it;
}

MaterialParamsBase::EGetParamResult MaterialParamsBase::getParamIndex(const String &name,
                                                                      MaterialParamsBase::EParamType type,
                                                                      EGpuParamDataType dataType,
                                                                      uint32_t arrayIdx, uint32_t &output) const {
    auto it = mParamLookup.find(name);
    if (it == nullptr) {
        return EGetParamResult::NotFound;
    }

    auto index = *it;
    const auto &param = mParams[index];

    if (param.type != type || (type == EParamType::Data && param.dataType != dataType)) {
        return EGetParamResult::InvalidType;
    }

    if (arrayIdx >= param.arraySize) {
        return EGetParamResult::IndexOutOfBounds;
    }

    output = index;
    return EGetParamResult::Success;
}

MaterialParamsBase::EGetParamResult MaterialParamsBase::getParamData(const String &name, EParamType type,
                                                                     EGpuParamDataType dataType, uint32_t arrayIdx,
                                                                     const ParamData **output) const {
    auto it = mParamLookup.find(name);
    if (it == nullptr) {
        return EGetParamResult::NotFound;
    }

    auto index = *it;
    const auto &param = mParams[index];
    *output = &param;

    if (param.type != type || (type == EParamType::Data && param.dataType != dataType)) {
        return EGetParamResult::InvalidType;
    }

    if (arrayIdx >= param.arraySize) {
        return EGetParamResult::IndexOutOfBounds;
    }

    return EGetParamResult::Success;
}

void MaterialParamsBase::reportGetParamError(MaterialParamsBase::EGetParamResult errorCode, const String &name,
                                             uint32_t arrayIdx) const {
    switch (errorCode) {
        case EGetParamResult::NotFound:
        LOG(LogMaterial, Warning, TEXT("Material doesn't have a parameter named %ls."), *name);
            break;
        case EGetParamResult::InvalidType:
        LOG(LogMaterial, Warning, TEXT("Parameter \"%ls\" is not of the requested type."), *name);
            break;
        case EGetParamResult::IndexOutOfBounds:
        LOG(LogMaterial, Warning, TEXT("Parameter \"%ls\" array index {1} out of range."), *name, arrayIdx);
            break;
        default:
            break;
    }
}

MaterialParams::MaterialParams(MaterialParams::ShaderType shader, uint64_t initialParamVersion)
        : MaterialParamsBase(shader->getDataParams(), shader->getTextureParams(), shader->getBufferParams(),
                             shader->getSamplerParams(), initialParamVersion){
    mStructParams.resize(mStructParamsCount);
    mTextureParams.resize(mTextureParamsCount);
    mBufferParams.resize(mBufferParamsCount);
    mSamplerStateParams.resize(mSamplerParamsCount);

    mDefaultTextureParams.resize(mTextureParamsCount);
    mDefaultSamplerStateParams.resize(mSamplerParamsCount);

    auto &textureParams = shader->getTextureParams();
    uint32_t textureIdx = 0;
    for (auto &entry : textureParams) {
        auto &param = mTextureParams[textureIdx];
        param.isLoadStore = false;

        if (entry.value.defaultValueIdx != static_cast<uint32_t>(-1)) {
            mDefaultTextureParams[textureIdx] = shader->getDefaultTexture(entry.value.defaultValueIdx);
        }

        textureIdx++;
    }

    auto &samplerParams = shader->getSamplerParams();
    uint32_t samplerIdx = 0;
    for (auto &entry : samplerParams) {
        if (entry.value.defaultValueIdx != static_cast<uint32_t>(-1)) {
            mDefaultSamplerStateParams[samplerIdx] = shader->getDefaultSampler(entry.value.defaultValueIdx);
        }

        samplerIdx++;
    }

    auto &dataParams = shader->getDataParams();
    auto &paramAttributes = shader->getParamAttributes();
    uint32_t structIdx = 0;
    for (auto &entry : dataParams) {
        if (entry.value.type == EGpuParamDataType::Struct) {
            uint32_t arraySize = entry.value.arraySize > 1 ? entry.value.arraySize : 1;
            for (uint32_t i = 0; i < arraySize; i++) {
                auto &param = mStructParams[structIdx];
                param.dataSize = entry.value.elementSize;
                param.data = q_alloc<uint8_t>(param.dataSize);

                structIdx++;
            }
        } else {
            uint32_t attribIdx = entry.value.attribIdx;
            while (attribIdx != static_cast<uint32_t>(-1)) {
                const auto &attrib = paramAttributes[attribIdx];
                if (attrib.type == EShaderParamAttributeType::SpriteUV) {
                    const auto texIt = mParamLookup.find(attrib.value);
                    const auto paramIt = mParamLookup.find(entry.key);
                    if (texIt != nullptr && paramIt != nullptr) {
                        auto &paramData = mParams[*paramIt];
                        auto &dataParamInfo = mDataParams[paramData.index];
                        dataParamInfo.spriteTextureIdx = *texIt;
                    }
                }

                attribIdx = attrib.nextParamIdx;
            }
        }
    }
}

MaterialParams::~MaterialParams() {
    if (!mStructParams.empty()) {
        for (uint32_t i = 0; i < mStructParamsCount; i++) {
            q_free(mStructParams[i].data);
        }

        mStructParams.clear();
    }

    mStructParams.clear();
    mTextureParams.clear();
    mBufferParams.clear();
    mStructParams.clear();

    if (!mDefaultTextureParams.empty()) {
        mDefaultTextureParams.clear();
    }

    if (!mDefaultSamplerStateParams.empty()) {
        mDefaultSamplerStateParams.clear();
    }
}

void MaterialParams::getStructData(const String &name, void *value, uint32_t size, uint32_t arrayIdx) const {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Data, EGpuParamDataType::Struct, arrayIdx, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, arrayIdx);
        return;
    }

    getStructData(*param, value, size, arrayIdx);
}

void MaterialParams::setStructData(const String &name, const void *value, uint32_t size, uint32_t arrayIdx) {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Data, EGpuParamDataType::Struct, arrayIdx, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, arrayIdx);
        return;
    }

    setStructData(*param, value, size, arrayIdx);
}

void MaterialParams::getTexture(const String &name, MaterialParams::TextureType &value, TextureSurface &surface) const {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Texture, EGpuParamDataType::Unknown, 0, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, 0);
        return;
    }

    getTexture(*param, value, surface);
}

void MaterialParams::setTexture(const String &name, MaterialParams::TextureType const &value,
                                const TextureSurface &surface) {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Texture, EGpuParamDataType::Unknown, 0, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, 0);
        return;
    }

    setTexture(*param, value, surface);
}

void MaterialParams::getBuffer(const String &name, MaterialParams::BufferType &value) const {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Buffer, EGpuParamDataType::Unknown, 0, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, 0);
        return;
    }

    getBuffer(*param, value);
}

void MaterialParams::setBuffer(const String &name, MaterialParams::BufferType const &value) {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Buffer, EGpuParamDataType::Unknown, 0, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, 0);
        return;
    }

    setBuffer(*param, value);
}

void MaterialParams::getSamplerState(const String &name, MaterialParams::SamplerType &value) const {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Sampler, EGpuParamDataType::Unknown, 0, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, 0);
        return;
    }

    getSamplerState(*param, value);
}

void MaterialParams::setSamplerState(const String &name, MaterialParams::SamplerType const &value) {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Sampler, EGpuParamDataType::Unknown, 0, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, 0);
        return;
    }

    setSamplerState(*param, value);
}

void MaterialParams::getStructData(const MaterialParamsBase::ParamData &param, void *value, uint32_t size,
                                   uint32_t arrayIdx) const {
    const MaterialParamStructData &structParam = mStructParams[param.index + arrayIdx];
    if (structParam.dataSize != size) {
        LOG(LogMaterial, Warning,
            TEXT("Size mismatch when writing to a struct. Provided size was %ld bytes but the struct ")
            TEXT("size is %ld bytes"), size, structParam.dataSize);
        return;
    }

    memcpy(value, structParam.data, structParam.dataSize);
}

void MaterialParams::setStructData(const MaterialParamsBase::ParamData &param, const void *value, uint32_t size,
                                   uint32_t arrayIdx) {
    const MaterialParamStructData &structParam = mStructParams[param.index + arrayIdx];
    if (structParam.dataSize != size) {
        LOG(LogMaterial, Warning,
            TEXT("Size mismatch when writing to a struct. Provided size was %ld bytes but the struct ")
            TEXT("size is %ld bytes"), size, structParam.dataSize);
        return;
    }

    std::memcpy(structParam.data, value, structParam.dataSize);
    param.version = ++mParamVersion;
}

uint32_t MaterialParams::getStructSize(const MaterialParamsBase::ParamData &param) const {
    const MaterialParamStructData &structParam = mStructParams[param.index];
    return structParam.dataSize;
}

void MaterialParams::getTexture(const MaterialParamsBase::ParamData &param, MaterialParams::TextureType &value,
                                TextureSurface &surface) const {
    const ParamTextureDataType &textureParam = mTextureParams[param.index];
    value = textureParam.texture;
    surface = textureParam.surface;
}

void MaterialParams::setTexture(const MaterialParamsBase::ParamData &param, MaterialParams::TextureType const &value,
                                const TextureSurface &surface) {
    ParamTextureDataType &textureParam = mTextureParams[param.index];
    textureParam.texture = value;
    textureParam.surface = surface;

    param.version = ++mParamVersion;
}

void MaterialParams::getBuffer(const MaterialParamsBase::ParamData &param, MaterialParams::BufferType &value) const {
    value = mBufferParams[param.index].value;
}

void MaterialParams::setBuffer(const MaterialParamsBase::ParamData &param, MaterialParams::BufferType const &value) {
    mBufferParams[param.index].value = value;
    param.version = ++mParamVersion;
}

void MaterialParams::getSamplerState(const MaterialParamsBase::ParamData &param, MaterialParams::SamplerType &value) const {
    value = mSamplerStateParams[param.index].value;
}

void MaterialParams::setSamplerState(const MaterialParamsBase::ParamData &param, MaterialParams::SamplerType const &value) {
    mSamplerStateParams[param.index].value = value;
    param.version = ++mParamVersion;
}

void MaterialParams::getDefaultTexture(const MaterialParamsBase::ParamData &param,
                                       MaterialParams::TextureType &value) const {
    value = mDefaultTextureParams[param.index];
}

void MaterialParams::getDefaultSamplerState(const MaterialParamsBase::ParamData &param,
                                            MaterialParams::SamplerType &value) const {
    value = mDefaultSamplerStateParams[param.index];
}

EMaterialParamTextureType MaterialParams::getTextureType(const MaterialParamsBase::ParamData &param) const {
    if (mTextureParams[param.index].isLoadStore)
        return EMaterialParamTextureType::LoadStore;

    return EMaterialParamTextureType::Normal;
}
