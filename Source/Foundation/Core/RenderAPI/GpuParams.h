#pragma once

#include "CorePrerequisites.h"
#include "GpuParamDesc.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix4.h"
#include "Image/Color.h"
#include "GpuPipelineParamInfo.h"
#include "GpuParam.h"
#include "Image/TextureSurface.h"
#include "Image/Texture.h"

struct GpuParamDataTypeInfo {
    uint32_t baseTypeSize;
    uint32_t size;
    uint32_t alignment;
    uint32_t numRows;
    uint32_t numColumns;
};

struct GpuDataParamInfos {
    GpuDataParamInfos() {
        memset(lookup, 0, sizeof(lookup));

        lookup[static_cast<uint32_t>(EGpuParamDataType::Float1)] = {4, 4, 4, 1, 1};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Float2)] = {4, 8, 8, 1, 2};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Float3)] = {4, 16, 16, 1, 3};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Float4)] = {4, 16, 16, 1, 4};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Color)] = {4, 16, 16, 1, 4};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Matrix2x2)] = {4, 16, 8, 2, 2};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Matrix2x3)] = {4, 32, 16, 2, 3};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Matrix2x4)] = {4, 32, 16, 2, 4};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Matrix3x2)] = {4, 24, 8, 3, 2};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Matrix3x3)] = {4, 48, 16, 3, 3};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Matrix3x4)] = {4, 48, 16, 3, 4};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Matrix4x2)] = {4, 32, 8, 4, 2};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Matrix4x3)] = {4, 64, 16, 4, 3};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Matrix4x4)] = {4, 64, 16, 4, 4};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Int1)] = {4, 4, 4, 1, 1};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Int2)] = {4, 8, 8, 1, 2};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Int3)] = {4, 12, 16, 1, 3};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Int4)] = {4, 16, 16, 1, 4};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Bool)] = {4, 4, 4, 1, 1};
        lookup[static_cast<uint32_t>(EGpuParamDataType::Struct)] = {4, 0, 16, 1, 1};
    }

    GpuParamDataTypeInfo lookup[static_cast<uint32_t>(EGpuParamDataType::Count)];
};

template<class T> struct TGpuDataParamInfo { enum { TypeId = static_cast<uint32_t>(EGpuParamDataType::Struct) };};

#define GPU_DATA_PARAM_INFO(TYPE, ENUM) \
    template<> struct TGpuDataParamInfo<TYPE> { enum { TypeId = static_cast<uint32_t>(EGpuParamDataType::ENUM) }; };

GPU_DATA_PARAM_INFO(float, Float1);
GPU_DATA_PARAM_INFO(Vector2, Float2);
GPU_DATA_PARAM_INFO(Vector3, Float3);
GPU_DATA_PARAM_INFO(int, Int1);
GPU_DATA_PARAM_INFO(Matrix4, Matrix4x4);
GPU_DATA_PARAM_INFO(Color, Color);

class DLL_EXPORT FGpuParamsBase {
protected:
    FGpuPipelineParamInfoBase *mParamInfo;

public:
    virtual ~FGpuParamsBase();

public:
    static GpuParams *New(FGpuPipelineParamInfo *paramInfo);
    static GpuParams *New(GraphicsPipelineState *pipelineState);

public:
    GpuParamDesc *getParamDesc(EGpuProgramType type) const;
    FGpuPipelineParamInfoBase *getParamInfo() const { return mParamInfo; }
    uint32_t getDataParamSize(EGpuProgramType type, const String &name) const;

    bool hasParam(EGpuProgramType type, const String &name) const;
    bool hasTexture(EGpuProgramType type, const String &name) const;
    bool hasLoadStoreTexture(EGpuProgramType type, const String &name) const;
    bool hasBuffer(EGpuProgramType type, const String &name) const;
    bool hasSamplerState(EGpuProgramType type, const String &name) const;
    bool hasParamBlock(EGpuProgramType type, const String &name) const;

    GpuParamBlockDesc *getParamBlockDesc(EGpuProgramType type, const String &name) const;

protected:
    FGpuParamsBase(FGpuPipelineParamInfoBase *paramInfo);

    GpuParamDataDesc *getParamDesc(EGpuProgramType type, const String &name) const;
};

class DLL_EXPORT GpuParams  : public FGpuParamsBase {
public:
    using GpuParamsType = GpuParams;

    using BufferType = GpuBuffer *;
    using ParamsBufferType = GpuParamBlockBuffer *;
    using TextureType = FResourceHandle<Texture>;
    using SamplerType = SamplerState *;

    struct TextureData {
        TextureType texture;
        TextureSurface surface;
    };

public:
    const static GpuDataParamInfos ParamSizes;

protected:
    ParamsBufferType *mParamBlockBuffers = nullptr;
    BufferType *mBuffers = nullptr;
    TextureData *mSampledTextureData = nullptr;
    SamplerType *mSamplerStates = nullptr;

public:
    GpuParams(FGpuPipelineParamInfoBase *paramInfo);
    virtual ~GpuParams();

public:
    template<class T>
    void getParam(EGpuProgramType type, const String &name, GpuDataParam<T> &output) const;
    void getBufferParam(EGpuProgramType type, const String &name, GpuParamBuffer &output) const;
    void getTextureParam(EGpuProgramType type, const String &name, GpuParamTexture &output) const;
    void getSamplerStateParam(EGpuProgramType type, const String &name, GpuParamSamplerState &output) const;

    ParamsBufferType getParamBlockBuffer(uint32_t set, uint32_t slot) const;
    BufferType getBuffer(uint32_t set, uint32_t slot) const;
    TextureType getTexture(uint32_t set, uint32_t slot) const;
    SamplerType getSamplerState(uint32_t set, uint32_t slot) const;
    const TextureSurface &getTextureSurface(uint32_t set, uint32_t slot) const;

    void setParamBlockBuffer(EGpuProgramType type, const String &name, const ParamsBufferType &paramBlockBuffer);
    void setParamBlockBuffer(const String &name, const ParamsBufferType &paramBlockBuffer);
    virtual void setParamBlockBuffer(uint32_t set, uint32_t slot, const ParamsBufferType &paramBlockBuffer);
    virtual void setBuffer(uint32_t set, uint32_t slot, const BufferType &buffer);
    virtual void setTexture(uint32_t set, uint32_t slot, TextureType texture, const TextureSurface &surface = TextureSurface::Complete);
    virtual void setSamplerState(uint32_t set, uint32_t slot, SamplerType sampler);

    template<class T>
    void setParam(EGpuProgramType type, const String &name, const T &value) {
        GpuDataParam<T> param; getParam(type, name, param); param.set(value);
    }

    void setBuffer(EGpuProgramType type, const String &name, const BufferType &buffer) {
        GpuParamBuffer param; getBufferParam(type, name, param); param.set(buffer);
    }

    void setTexture(EGpuProgramType type, const String &name, TextureType texture, const TextureSurface &surface = TextureSurface::Complete) {
        GpuParamTexture param; getTextureParam(type, name, param); param.set(texture, surface);
    }

    void setSamplerState(EGpuProgramType type, const String &name, const SamplerType &sampler) {
        GpuParamSamplerState param; getSamplerStateParam(type, name, param); param.set(sampler);
    }
};