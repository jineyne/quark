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

struct FindTechniqueDesc {
    static constexpr uint32_t MaxNumTags = 10;

    String tags[MaxNumTags];
    uint32_t tagCount;
    const ShaderVariation *variation = nullptr;
    bool override = false;

    void addTag(const String &tag) {
        assert(tagCount < MaxNumTags);

        tags[tagCount] = tag;
        tagCount++;
    }
};

class DLL_EXPORT MaterialBase {
public:
    struct StructData {
    public:
        void *data;
        uint32_t size;

    public:
        StructData() : data(nullptr), size(0) {}

        StructData(uint32_t size) : size(size) {
            data = (void *) q_new<uint8_t>(size);
        }

        ~StructData() {
            q_delete((uint8_t *) data);
        }
    };

public:
    MaterialBase() = default;
    virtual ~MaterialBase() = default;

public:
    virtual void markCoreDirty(EMaterialDirtyFlags flags = EMaterialDirtyFlags::Param) {}
    virtual void markDependenciesDirty() {}
    virtual void markResourcesDirty() {}
};


QCLASS()
class DLL_EXPORT Material : public Resource, public MaterialBase {
public:
    using TextureType = FResourceHandle<Texture>;
    using BufferType = GpuBuffer *;
    using SamplerStateType = SamplerState *;
    using GpuProgramType = GpuProgram *;
    using PassType = Pass;
    using TechniqueType = Technique;
    using ShaderType = Shader;
    using GpuParamsSetType = GpuParamsSet;
    using MaterialParamsType = MaterialParams;

protected:
    QPROPERTY()
    ShaderType *mShader = nullptr;

    QPROPERTY()
    MaterialParamsType *mParams = nullptr;

    QPROPERTY()
    TArray<TechniqueType *> mTechniqueList;

    QPROPERTY()
    ShaderVariation mVariation;

    uint32_t mLoadFlags;

public:
    Material(ShaderType *shader, const ShaderVariation &variation);
    virtual ~Material();

public:
    static Material *New(ShaderType *shader, const ShaderVariation &variation = ShaderVariation());

public:

#pragma region State Update

    uint32_t findTechnique(FindTechniqueDesc &desc) const;

    void updateParamsSet(GpuParamsSetType *paramsSet, float t = 0.0f, bool updateAll = false);

    GpuParamsSetType *createParamsSet(uint32_t techniqueIdx = 0);

#define MATERIAL_DATA_PARAM_SET(TYPE, SHORT_NAME) \
    void set##SHORT_NAME(const String &name, const TYPE &value, uint32_t arrayIdx = 0) { \
        getParam##SHORT_NAME(name).set(value, arrayIdx); \
    }

    MATERIAL_DATA_PARAM_SET(float, Float);
    MATERIAL_DATA_PARAM_SET(Color, Color);
    MATERIAL_DATA_PARAM_SET(Vector2, Vec2);
    MATERIAL_DATA_PARAM_SET(Vector3, Vec3);
    MATERIAL_DATA_PARAM_SET(Matrix4, Mat4);

    void setTexture(const String &name, const TextureType &value, const TextureSurface &surface = TextureSurface::Complete) {
        return getParamTexture(name).set(value, surface);
    }

    void setSamplerState(const String &name, const SamplerStateType &value) {
        return getParamSamplerState(name).set(value);
    }

    void setBuffer(const String &name, const BufferType &value) { return getParamBuffer(name).set(value); }

    void setStructData(const String &name, void *value, uint32_t size, uint32_t arrayIdx = 0) {
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
    void getParam(const String &name, TMaterialDataParam<T> &output) const;

#define MATERIAL_DATA_PARAM_DECL(TYPE, SHORT_NAME) \
    TMaterialDataParam<TYPE> getParam##SHORT_NAME(const String &name) const { \
        TMaterialDataParam<TYPE> result; getParam(name, result); return result;  \
    }

    MATERIAL_DATA_PARAM_DECL(float, Float);
    MATERIAL_DATA_PARAM_DECL(Color, Color);
    MATERIAL_DATA_PARAM_DECL(Vector2, Vec2);
    MATERIAL_DATA_PARAM_DECL(Vector3, Vec3);
    MATERIAL_DATA_PARAM_DECL(Matrix4, Mat4);

    MaterialParamStruct getParamStruct(const String &name) const;
    MaterialParamTexture getParamTexture(const String &name) const;
    MaterialParamBuffer getParamBuffer(const String &name) const;
    MaterialParamSamplerState getParamSamplerState(const String &name) const;

    float getFloat(const String &name, uint32_t arrayIdx = 0) const { return getParamFloat(name).get(arrayIdx); }
    Color getColor(const String &name, uint32_t arrayIdx = 0) const { return getParamColor(name).get(arrayIdx); }
    Vector2 getVec2(const String &name, uint32_t arrayIdx = 0) const { return getParamVec2(name).get(arrayIdx); }
    Vector3 getVec3(const String &name, uint32_t arrayIdx = 0) const { return getParamVec3(name).get(arrayIdx); }
    Matrix4 getMat4(const String &name, uint32_t arrayIdx = 0) const { return getParamMat4(name).get(arrayIdx); }

    TextureType getTexture(const String &name) const { return getParamTexture(name).get(); }

    SamplerStateType getSamplerState(const String &name) const { return getParamSamplerState(name).get(); }

    MaterialBase::StructData getStructData(const String &name, uint32_t arrayIdx = 0) const {
        MaterialParamStruct structParam = getParamStruct(name);
        MaterialBase::StructData data(structParam.getElementSize());
        structParam.get(data.data, structParam.getElementSize(), arrayIdx);

        return data;
    }

#pragma endregion

protected:
    Material() = default;

    void initializeTechniques();
    void initDefaultParameters();
    void throwIfNotInitialized() const;

private:
    void setParams(MaterialParams *params);
};