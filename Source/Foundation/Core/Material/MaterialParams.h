#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/GpuParamDesc.h"
#include "RenderAPI/GpuParams.h"
#include "Shader.h"

enum class EMaterialParamTextureType {
    Normal,
    LoadStore,
    Sprite
};

class DLL_EXPORT MaterialParamsBase {
public:
    enum class EParamType {
        Data, Texture, Sampler, Buffer
    };

    enum class EGetParamResult {
        Success, NotFound, InvalidType, IndexOutOfBounds
    };

    struct ParamData {
        EParamType type;
        EGpuParamDataType dataType;
        uint32_t index;
        uint32_t arraySize;
        mutable uint64_t version;
    };

    struct DataParamInfo {
        uint32_t offset;

        // TODO: animation

        uint32_t spriteTextureIdx;
    };

protected:
    const static uint32_t StaticBufferSize = 256;

    TMap<String, uint32_t> mParamLookup;
    TArray<ParamData> mParams;

    TArray<DataParamInfo> mDataParams;
    TArray<uint8_t> mDataParamsBuffer;

    uint32_t mDataSize = 0;
    uint32_t mDataParamsCount = 0;
    uint32_t mStructParamsCount = 0;
    uint32_t mTextureParamsCount = 0;
    uint32_t mBufferParamsCount = 0;
    uint32_t mSamplerParamsCount = 0;

    mutable uint64_t mParamVersion = 1;

public:
    MaterialParamsBase(const TMap<String, ShaderDataParamDesc> &dataParams,
                       const TMap<String, ShaderObjectParamDesc> &textureParams,
                       const TMap<String, ShaderObjectParamDesc> &bufferParams,
                       const TMap<String, ShaderObjectParamDesc> &samplerParams,
                       uint64_t initialParamVersion);

    MaterialParamsBase() = default;

    virtual ~MaterialParamsBase();

    template<typename T>
    void getDataParam(const String &name, uint32_t arrayIdx, T &output) const {
        auto dataType = static_cast<EGpuParamDataType>(TGpuDataParamInfo<T>::TypeId);

        const ParamData *param = nullptr;
        auto result = getParamData(name, EParamType::Data, dataType, arrayIdx, &param);
        if (result != EGetParamResult::Success) {
            return;
        }

        const auto &paramInfo = mDataParams[param->index + arrayIdx];

        const GpuParamDataTypeInfo &typeInfo = GpuParams::ParamSizes.lookup[static_cast<uint32_t>(dataType)];
        uint32_t paramTypeSize = typeInfo.numColumns * typeInfo.numRows * typeInfo.baseTypeSize;

        std::memcpy(output, &mDataParamsBuffer[paramInfo.offset], sizeof(paramTypeSize));
    }

    template<typename T>
    void setDataParam(const String &name, uint32_t arrayIdx, const T &input) const {
        auto dataType = static_cast<EGpuParamDataType>(TGpuDataParamInfo<T>::TypeId);

        const ParamData *param = nullptr;
        auto result = getParamData(name, EParamType::Data, dataType, arrayIdx, &param);
        if (result != EGetParamResult::Success) {
            return;
        }

        const auto &paramInfo = mDataParams[param->index + arrayIdx];

        const GpuParamDataTypeInfo &typeInfo = GpuParams::ParamSizes.lookup[static_cast<uint32_t>(dataType)];
        uint32_t paramTypeSize = typeInfo.numColumns * typeInfo.numRows * typeInfo.baseTypeSize;

        std::memcpy(&mDataParamsBuffer[paramInfo.offset], input, sizeof(paramTypeSize));
    }

    uint32_t getParamIndex(const String &name) const;

    EGetParamResult getParamIndex(const String &name, EParamType type, EGpuParamDataType dataType,
                                  uint32_t arrayIdx, uint32_t &output) const;

    EGetParamResult getParamData(const String &name, EParamType type, EGpuParamDataType dataType,
                                 uint32_t arrayIdx, const ParamData **output) const;

    const ParamData *getParamData(uint32_t index) const { return &mParams[index]; }

    uint32_t getParamsCount() const { return static_cast<uint32_t>(mParams.length()); }

    template<typename T>
    void getDataParam(const ParamData &param, uint32_t arrayIdx, T &output) const {
        auto dataType = static_cast<EGpuParamDataType>(TGpuDataParamInfo<T>::TypeId);

        const auto &paramInfo = mDataParams[param.index + arrayIdx];

        const GpuParamDataTypeInfo &typeInfo = GpuParams::ParamSizes.lookup[static_cast<uint32_t>(dataType)];
        uint32_t paramTypeSize = typeInfo.numColumns * typeInfo.numRows * typeInfo.baseTypeSize;

        assert(sizeof(output) == paramTypeSize);
        std::memcpy(&output, &mDataParamsBuffer[paramInfo.offset], paramTypeSize);
    }

    template<typename T>
    void setDataParam(const ParamData &param, uint32_t arrayIdx, const T &input) const {
        auto dataType = static_cast<EGpuParamDataType>(TGpuDataParamInfo<T>::TypeId);

        const auto &paramInfo = mDataParams[param.index + arrayIdx];

        // TODO: animation

        const GpuParamDataTypeInfo &typeInfo = GpuParams::ParamSizes.lookup[static_cast<uint32_t>(dataType)];
        uint32_t paramTypeSize = typeInfo.numColumns * typeInfo.numRows * typeInfo.baseTypeSize;

        assert(sizeof(input) == paramTypeSize);
        std::memcpy(const_cast<uint8_t *>(&mDataParamsBuffer[paramInfo.offset]), &input, paramTypeSize);

        param.version = ++mParamVersion;
    }

    const uint8_t *getData(uint32_t index) const {
        return &mDataParamsBuffer[mDataParams[index].offset];
    }

    uint64_t getParamVersion() const { return mParamVersion; }

    void reportGetParamError(EGetParamResult errorCode, const String &name, uint32_t arrayIdx) const;
};

class DLL_EXPORT MaterialParamStructData {
public:
    uint32_t dataSize;
    uint8_t *data;
};

class DLL_EXPORT MaterialParamTextureData {
public:
    FResourceHandle<Texture> texture;
    bool isLoadStore;
    TextureSurface surface;
};

class DLL_EXPORT MaterialParamBufferData {
public:
    GpuBuffer *value;
};

class DLL_EXPORT MaterialParamSamplerStateData {
public:
    SamplerState *value;
};

class DLL_EXPORT MaterialParams : public MaterialParamsBase {
public:
    using GpuParamsType = GpuParams;
    using TextureType = FResourceHandle<Texture>;
    using ShaderType = Shader *;
    using BufferType = GpuBuffer *;
    using SamplerType = SamplerState *;

    using ParamStructDataType = MaterialParamStructData;
    using ParamTextureDataType = MaterialParamTextureData;
    using ParamBufferDataType = MaterialParamBufferData;
    using ParamSamplerStateDataType = MaterialParamSamplerStateData;

protected:
    TArray<ParamStructDataType> mStructParams;
    TArray<ParamTextureDataType> mTextureParams;
    TArray<ParamBufferDataType> mBufferParams;
    TArray<ParamSamplerStateDataType> mSamplerStateParams;
    TArray<TextureType> mDefaultTextureParams;
    TArray<SamplerType> mDefaultSamplerStateParams;

public:
    MaterialParams(ShaderType shader, uint64_t initialParamVersion);
    MaterialParams() = default;

    virtual ~MaterialParams();

public:
    void getStructData(const String &name, void *value, uint32_t size, uint32_t arrayIdx) const;
    void setStructData(const String &name, const void *value, uint32_t size, uint32_t arrayIdx);

    void getTexture(const String &name, TextureType &value, TextureSurface &surface) const;
    void setTexture(const String &name, const TextureType &value,
                    const TextureSurface &surface = TextureSurface::Complete);

    void getBuffer(const String &name, BufferType &value) const;
    void setBuffer(const String &name, const BufferType &value);

    void getSamplerState(const String &name, SamplerType &value) const;
    void setSamplerState(const String &name, const SamplerType &value);

    void getStructData(const ParamData &param, void *value, uint32_t size, uint32_t arrayIdx) const;
    void setStructData(const ParamData &param, const void *value, uint32_t size, uint32_t arrayIdx);

    uint32_t getStructSize(const ParamData &param) const;

    void getTexture(const ParamData &param, TextureType &value, TextureSurface &surface) const;
    void setTexture(const ParamData &param, const TextureType &value,
                    const TextureSurface &surface = TextureSurface::Complete);

    void getBuffer(const ParamData &param, BufferType &value) const;
    void setBuffer(const ParamData &param, const BufferType &value);

    void getSamplerState(const ParamData &param, SamplerType &value) const;
    void setSamplerState(const ParamData &param, const SamplerType &value);

    void getDefaultTexture(const ParamData &param, TextureType &value) const;
    void getDefaultSamplerState(const ParamData &param, SamplerType &value) const;
    EMaterialParamTextureType getTextureType(const ParamData &param) const;
};