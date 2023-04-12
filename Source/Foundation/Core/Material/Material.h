#pragma once

#include "CorePrerequisites.h"
#include "MaterialType.h"
#include "ShaderVariation.h"
#include "Pass.h"
#include "Image/Texture.h"
#include "Technique.h"
#include "Shader.h"
#include "MaterialParams.h"
#include "GpuParamsSet.h"
#include "MaterialParam.h"

enum class EMaterialDirtyFlags {
    Param = 1 << 0,
    ParamResource = 1 << 1,
    Shader = 2 << 2
};

ENUM_CLASS_FLAGS(EMaterialDirtyFlags)

struct FFindTechniqueDesc {
    static constexpr uint32_t MaxNumTags = 10;

    FString tags[MaxNumTags];
    uint32_t tagCount;
    const FShaderVariation *variation = nullptr;
    bool override = false;

    void addTag(const FString &tag) {
        assert(tagCount < MaxNumTags);

        tags[tagCount] = tag;
        tagCount++;
    }
};

class DLL_EXPORT FMaterialBase {
public:
    struct FStructData {
    public:
        void *data;
        uint32_t size;

    public:
        FStructData() : data(nullptr), size(0) {}

        FStructData(uint32_t size) : size(size) {
            data = (void *) q_new<uint8_t>(size);
        }

        ~FStructData() {
            q_delete((uint8_t *) data);
        }
    };

public:
    FMaterialBase() = default;
    virtual ~FMaterialBase() = default;

public:
    virtual void markCoreDirty(EMaterialDirtyFlags flags = EMaterialDirtyFlags::Param) {}
    virtual void markDependenciesDirty() {}
    virtual void markResourcesDirty() {}
};


class DLL_EXPORT FMaterial : public FMaterialBase {
public:
    using TextureType = FResourceHandle<FTexture>;
    using BufferType = FGpuBuffer *;
    using SamplerStateType = FSamplerState *;
    using GpuProgramType = FGpuProgram *;
    using PassType = FPass;
    using TechniqueType = FTechnique;
    using ShaderType = FShader;
    using GpuParamsSetType = FGpuParamsSet;
    using MaterialParamsType = FMaterialParams;

protected:
    ShaderType *mShader = nullptr;
    MaterialParamsType *mParams = nullptr;

    TArray<TechniqueType *> mTechniqueList;
    FShaderVariation mVariation;
    uint32_t mLoadFlags;

public:
    FMaterial(ShaderType *shader, const FShaderVariation &variation);
    virtual ~FMaterial();

public:
    static FMaterial *New(ShaderType *shader, const FShaderVariation &variation = FShaderVariation());

public:

#pragma region State Update

    uint32_t findTechnique(FFindTechniqueDesc &desc) const;

    void updateParamsSet(GpuParamsSetType *paramsSet, float t = 0.0f, bool updateAll = false);

    GpuParamsSetType *createParamsSet(uint32_t techniqueIdx = 0);

#define MATERIAL_DATA_PARAM_SET(TYPE, SHORT_NAME) \
    void set##SHORT_NAME(const FString &name, const F##TYPE &value, uint32_t arrayIdx = 0) { \
        getParam##SHORT_NAME(name).set(value, arrayIdx); \
    }

#define MATERIAL_DATA_PARAM_SET_PRIMARY(TYPE, SHORT_NAME) \
    void set##SHORT_NAME(const FString &name, const TYPE &value, uint32_t arrayIdx = 0) { \
        getParam##SHORT_NAME(name).set(value, arrayIdx); \
    }

    MATERIAL_DATA_PARAM_SET_PRIMARY(float, Float);
    MATERIAL_DATA_PARAM_SET(Color, Color);
    MATERIAL_DATA_PARAM_SET(Vector2, Vec2);
    MATERIAL_DATA_PARAM_SET(Vector3, Vec3);
    MATERIAL_DATA_PARAM_SET(Matrix4, Mat4);

    void setTexture(const FString &name, const TextureType &value, const FTextureSurface &surface = FTextureSurface::Complete) {
        return getParamTexture(name).set(value, surface);
    }

    void setSamplerState(const FString &name, const SamplerStateType &value) {
        return getParamSamplerState(name).set(value);
    }

    void setBuffer(const FString &name, const BufferType &value) { return getParamBuffer(name).set(value); }

    void setStructData(const FString &name, void *value, uint32_t size, uint32_t arrayIdx = 0) {
        return getParamStruct(name).set(value, size, arrayIdx);
    }

#pragma endregion State Update

#pragma region Getter

    const auto &getShader() const { return mShader; }

    const auto &getVariation() const { return mVariation; }

    uint32_t getTechniqueSize() const { return static_cast<uint32_t>(mTechniqueList.length()); }

    const auto &getTechnique(uint32_t idx) const { return mTechniqueList[idx]; }

    uint32_t getPassesCount(uint32_t techniqueIdx = 0) const;
    PassType *getPass(uint32_t passIdx = 0, uint32_t techniqueIdx = 0) const;

    const auto &getParams() const { return mParams; }

    uint32_t getDefaultTechnique() const;

    template<typename T>
    void getParam(const FString &name, TMaterialDataParam<T> &output) const;

#define MATERIAL_DATA_PARAM_DECL(TYPE, SHORT_NAME) \
    TMaterialDataParam<F##TYPE> getParam##SHORT_NAME(const FString &name) const { \
        TMaterialDataParam<F##TYPE> result; getParam(name, result); return result;  \
    }

#define MATERIAL_DATA_PARAM_DECL_PRIMARY(TYPE, SHORT_NAME) \
    TMaterialDataParam<TYPE> getParam##SHORT_NAME(const FString &name) const { \
        TMaterialDataParam<TYPE> result; getParam(name, result); return result;  \
    }

    MATERIAL_DATA_PARAM_DECL_PRIMARY(float, Float);
    MATERIAL_DATA_PARAM_DECL(Color, Color);
    MATERIAL_DATA_PARAM_DECL(Vector2, Vec2);
    MATERIAL_DATA_PARAM_DECL(Vector3, Vec3);
    MATERIAL_DATA_PARAM_DECL(Matrix4, Mat4);

    MaterialParamStruct getParamStruct(const FString &name) const;
    MaterialParamTexture getParamTexture(const FString &name) const;
    MaterialParamBuffer getParamBuffer(const FString &name) const;
    MaterialParamSamplerState getParamSamplerState(const FString &name) const;

    float getFloat(const FString &name, uint32_t arrayIdx = 0) const { return getParamFloat(name).get(arrayIdx); }
    FColor getColor(const FString &name, uint32_t arrayIdx = 0) const { return getParamColor(name).get(arrayIdx); }
    FVector2 getVec2(const FString &name, uint32_t arrayIdx = 0) const { return getParamVec2(name).get(arrayIdx); }
    FVector3 getVec3(const FString &name, uint32_t arrayIdx = 0) const { return getParamVec3(name).get(arrayIdx); }
    FMatrix4 getMat4(const FString &name, uint32_t arrayIdx = 0) const { return getParamMat4(name).get(arrayIdx); }

    TextureType getTexture(const FString &name) const { return getParamTexture(name).get(); }

    SamplerStateType getSamplerState(const FString &name) const { return getParamSamplerState(name).get(); }

    FMaterialBase::FStructData getStructData(const FString &name, uint32_t arrayIdx = 0) const {
        MaterialParamStruct structParam = getParamStruct(name);
        FMaterialBase::FStructData data(structParam.getElementSize());
        structParam.get(data.data, structParam.getElementSize(), arrayIdx);

        return data;
    }

#pragma endregion

protected:
    FMaterial() = default;

    void initializeTechniques();
    void initDefaultParameters();
    void throwIfNotInitialized() const;

private:
    void setParams(FMaterialParams *params);
};