#include "MaterialParams.h"

#include "Material/Shader.h"
#include "Image/Texture.h"
#include "RenderAPI/GpuBuffer.h"
#include "RenderAPI/SamplerState.h"

FMaterialParamsBase::FMaterialParamsBase(const TMap<FString, FShaderDataParamDesc> &dataParams,
                                         const TMap<FString, FShaderObjectParamDesc> &textureParams,
                                         const TMap<FString, FShaderObjectParamDesc> &bufferParams,
                                         const TMap<FString, FShaderObjectParamDesc> &samplerParams,
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
            const auto &typeInfo = FGpuParams::ParamSizes.lookup[static_cast<uint32_t>(param.value.type)];
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

            const GpuParamDataTypeInfo &typeInfo = FGpuParams::ParamSizes.lookup[(int) dataParam.dataType];
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

FMaterialParamsBase::~FMaterialParamsBase() {
        mDataParamsBuffer.clear();
        mDataParams.clear();
}

uint32_t FMaterialParamsBase::getParamIndex(const FString &name) const {
    auto it = mParamLookup.find(name);
    if (it == nullptr) {
        return static_cast<uint32_t>(-1);
    }

    return *it;
}

FMaterialParamsBase::EGetParamResult FMaterialParamsBase::getParamIndex(const FString &name,
                                                                        FMaterialParamsBase::EParamType type,
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

FMaterialParamsBase::EGetParamResult FMaterialParamsBase::getParamData(const FString &name, EParamType type,
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

void FMaterialParamsBase::reportGetParamError(FMaterialParamsBase::EGetParamResult errorCode, const FString &name,
                                              uint32_t arrayIdx) const {
    switch (errorCode) {
        case EGetParamResult::NotFound:
        LOG(FLogMaterial, Warning, TEXT("FMaterial doesn't have a parameter named %ls."), *name);
            break;
        case EGetParamResult::InvalidType:
        LOG(FLogMaterial, Warning, TEXT("Parameter \"%ls\" is not of the requested type."), *name);
            break;
        case EGetParamResult::IndexOutOfBounds:
        LOG(FLogMaterial, Warning, TEXT("Parameter \"%ls\" array index {1} out of range."), *name, arrayIdx);
            break;
        default:
            break;
    }
}

FMaterialParams::FMaterialParams(FMaterialParams::ShaderType shader, uint64_t initialParamVersion)
        : FMaterialParamsBase(shader->getDataParams(), shader->getTextureParams(), shader->getBufferParams(),
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
                if (attrib.type == ShaderParamAttributeType::SpriteUV) {
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

FMaterialParams::~FMaterialParams() {
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

void FMaterialParams::getStructData(const FString &name, void *value, uint32_t size, uint32_t arrayIdx) const {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Data, EGpuParamDataType::Struct, arrayIdx, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, arrayIdx);
        return;
    }

    getStructData(*param, value, size, arrayIdx);
}

void FMaterialParams::setStructData(const FString &name, const void *value, uint32_t size, uint32_t arrayIdx) {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Data, EGpuParamDataType::Struct, arrayIdx, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, arrayIdx);
        return;
    }

    setStructData(*param, value, size, arrayIdx);
}

void FMaterialParams::getTexture(const FString &name, FMaterialParams::TextureType &value, FTextureSurface &surface) const {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Texture, EGpuParamDataType::Unknown, 0, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, 0);
        return;
    }

    getTexture(*param, value, surface);
}

void FMaterialParams::setTexture(const FString &name, FMaterialParams::TextureType const &value,
                                 const FTextureSurface &surface) {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Texture, EGpuParamDataType::Unknown, 0, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, 0);
        return;
    }

    setTexture(*param, value, surface);
}

void FMaterialParams::getBuffer(const FString &name, FMaterialParams::BufferType &value) const {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Buffer, EGpuParamDataType::Unknown, 0, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, 0);
        return;
    }

    getBuffer(*param, value);
}

void FMaterialParams::setBuffer(const FString &name, FMaterialParams::BufferType const &value) {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Buffer, EGpuParamDataType::Unknown, 0, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, 0);
        return;
    }

    setBuffer(*param, value);
}

void FMaterialParams::getSamplerState(const FString &name, FMaterialParams::SamplerType &value) const {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Sampler, EGpuParamDataType::Unknown, 0, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, 0);
        return;
    }

    getSamplerState(*param, value);
}

void FMaterialParams::setSamplerState(const FString &name, FMaterialParams::SamplerType const &value) {
    const ParamData *param = nullptr;
    auto result = getParamData(name, EParamType::Sampler, EGpuParamDataType::Unknown, 0, &param);
    if (result != EGetParamResult::Success) {
        reportGetParamError(result, name, 0);
        return;
    }

    setSamplerState(*param, value);
}

void FMaterialParams::getStructData(const FMaterialParamsBase::ParamData &param, void *value, uint32_t size,
                                    uint32_t arrayIdx) const {
    const FMaterialParamStructData &structParam = mStructParams[param.index + arrayIdx];
    if (structParam.dataSize != size) {
        LOG(FLogMaterial, Warning,
            TEXT("Size mismatch when writing to a struct. Provided size was %ld bytes but the struct ")
            TEXT("size is %ld bytes"), size, structParam.dataSize);
        return;
    }

    memcpy(value, structParam.data, structParam.dataSize);
}

void FMaterialParams::setStructData(const FMaterialParamsBase::ParamData &param, const void *value, uint32_t size,
                                    uint32_t arrayIdx) {
    const FMaterialParamStructData &structParam = mStructParams[param.index + arrayIdx];
    if (structParam.dataSize != size) {
        LOG(FLogMaterial, Warning,
            TEXT("Size mismatch when writing to a struct. Provided size was %ld bytes but the struct ")
            TEXT("size is %ld bytes"), size, structParam.dataSize);
        return;
    }

    std::memcpy(structParam.data, value, structParam.dataSize);
    param.version = ++mParamVersion;
}

uint32_t FMaterialParams::getStructSize(const FMaterialParamsBase::ParamData &param) const {
    const FMaterialParamStructData &structParam = mStructParams[param.index];
    return structParam.dataSize;
}

void FMaterialParams::getTexture(const FMaterialParamsBase::ParamData &param, FMaterialParams::TextureType &value,
                                 FTextureSurface &surface) const {
    const ParamTextureDataType &textureParam = mTextureParams[param.index];
    value = textureParam.texture;
    surface = textureParam.surface;
}

void FMaterialParams::setTexture(const FMaterialParamsBase::ParamData &param, FMaterialParams::TextureType const &value,
                                 const FTextureSurface &surface) {
    ParamTextureDataType &textureParam = mTextureParams[param.index];
    textureParam.texture = value;
    textureParam.surface = surface;

    param.version = ++mParamVersion;
}

void FMaterialParams::getBuffer(const FMaterialParamsBase::ParamData &param, FMaterialParams::BufferType &value) const {
    value = mBufferParams[param.index].value;
}

void FMaterialParams::setBuffer(const FMaterialParamsBase::ParamData &param, FMaterialParams::BufferType const &value) {
    mBufferParams[param.index].value = value;
    param.version = ++mParamVersion;
}

void FMaterialParams::getSamplerState(const FMaterialParamsBase::ParamData &param, FMaterialParams::SamplerType &value) const {
    value = mSamplerStateParams[param.index].value;
}

void FMaterialParams::setSamplerState(const FMaterialParamsBase::ParamData &param, FMaterialParams::SamplerType const &value) {
    mSamplerStateParams[param.index].value = value;
    param.version = ++mParamVersion;
}

void FMaterialParams::getDefaultTexture(const FMaterialParamsBase::ParamData &param,
                                        FMaterialParams::TextureType &value) const {
    value = mDefaultTextureParams[param.index];
}

void FMaterialParams::getDefaultSamplerState(const FMaterialParamsBase::ParamData &param,
                                             FMaterialParams::SamplerType &value) const {
    value = mDefaultSamplerStateParams[param.index];
}

EMaterialParamTextureType FMaterialParams::getTextureType(const FMaterialParamsBase::ParamData &param) const {
    if (mTextureParams[param.index].isLoadStore)
        return EMaterialParamTextureType::LoadStore;

    return EMaterialParamTextureType::Normal;
}
