#pragma once

#include "CorePrerequisites.h"
#include "Image/Texture.h"
#include "RenderAPI/GpuParamDesc.h"
#include "Technique.h"
#include "Renderer/RendererTypes.h"
#include "ShaderVariation.h"

enum class EGpuParamType {
    Data,
    Texture,
    Buffer,
    Sampler
};

enum class ShaderParamAttributeType {
    SpriteUV,
    Name,
    HDR
};

struct FShaderParamBase {
    FString name;
    FString gpuVariableName;
    FString rendererSemantic;
    uint32_t defaultValueIdx = static_cast<uint32_t>(-1);
    uint32_t attribIdx = static_cast<uint32_t>(-1);

    FShaderParamBase() = default;
    FShaderParamBase(const FString &name, const FString &gpuVariableName, FString rendererSemantic = TEXT(""))
            : name(name), gpuVariableName(gpuVariableName), rendererSemantic(rendererSemantic) {}
};

struct FShaderDataParamDesc : public FShaderParamBase {
    EGpuParamDataType type = EGpuParamDataType::Float1;
    uint32_t arraySize = 1;
    uint32_t elementSize = 0;

    FShaderDataParamDesc() = default;
    FShaderDataParamDesc(const FString &name, const FString &gpuVariableName, EGpuParamDataType type,
                         uint32_t arraySize = 1, uint32_t elementSize = 0)
            : FShaderParamBase(name, gpuVariableName), type(type), arraySize(arraySize), elementSize(elementSize) {}
};

struct FShaderObjectParamDesc : public FShaderParamBase {
    EGpuParamObjectType type = EGpuParamObjectType::Texture2D;
    TArray<FString> gpuVariableNames;

    FShaderObjectParamDesc() = default;
    FShaderObjectParamDesc(const FString & name, const FString & gpuVariableName, EGpuParamObjectType type)
            : FShaderParamBase(name, gpuVariableName), type(type) {
        gpuVariableNames.add(gpuVariableName);
    }
};

struct FShaderParamBlockDesc {
    FString name;
    bool shared;
    FString rendererSemantic;
    EBufferUsage usage;
};

struct FShaderParamAttribute {
    ShaderParamAttributeType type = ShaderParamAttributeType::SpriteUV;
    FString value;
    uint32_t nextParamIdx = static_cast<uint32_t>(-1);
};

struct ShaderVariationParamValue {
    FString name;
    int32_t value = 0;
};

struct ShaderVariationParamInfo {
    FString name;
    FString identifier;
    bool isInternal = true;
    TArray<ShaderVariationParamValue> values;
};

class DLL_EXPORT FShaderDesc {
public:
    using TextureType = FResourceHandle<FTexture>;
    using SamplerStateType = FSamplerState *;
    using TechniqueType = FTechnique *;

public:
    TArray<TechniqueType> techniques;
    TArray<ShaderVariationParamInfo> variationParams;

    TMap<FString, FShaderDataParamDesc> dataParams;
    TMap<FString, FShaderObjectParamDesc> textureParams;
    TMap<FString, FShaderObjectParamDesc> bufferParams;
    TMap<FString, FShaderObjectParamDesc> samplerParams;
    TMap<FString, FShaderParamBlockDesc> paramBlocks;

    TArray<uint8_t> dataDefaultValues;
    TArray<SamplerStateType> samplerDefaultValues;
    TArray<TextureType> textureDefaultValues;
    TArray<FShaderParamAttribute> paramAttributes;

    EQueueSortType queueSortType = EQueueSortType::None;

    int32_t queuPriority = 0;
    bool separablePasses = false;

public:
    FShaderDesc();

public:
    void addParameter(FShaderDataParamDesc paramDesc, uint32_t *defaultValue = nullptr);
    void addParameter(FShaderObjectParamDesc paramDesc);
    void addParameter(FShaderObjectParamDesc paramDesc, const SamplerStateType &defaultValue);
    void addParameter(FShaderObjectParamDesc paramDesc, const TextureType &defaultValue);
    void addParameterAttribute(const FString &name, const FShaderParamAttribute &attrib);
    void setParamBlockAttribs(const FString &name, bool shared, EBufferUsage usage, FString rendererSemantic);

private:
    void addParameterInternal(FShaderObjectParamDesc paramDesc, uint32_t defaultValueIndex);
};

class DLL_EXPORT FShader : public FResource {
public:
    using TechniqueType = FShaderDesc::TechniqueType;
    using TextureType = FShaderDesc::TextureType;
    using SamplerStateType = FShaderDesc::SamplerStateType;

    static uint32_t mNextShaderId;

protected:
    FString mName;

    FShaderDesc mDesc;

    uint32_t mId;

public:
    FShader(uint32_t id);

    FShader(const FString &name, const FShaderDesc &desc, uint32_t id);

    virtual ~FShader() = default;

public:
    static FShader *New(const FString &name, const FShaderDesc &desc);

public:
    static bool IsSampler(EGpuParamObjectType type);
    static bool IsTexture(EGpuParamObjectType type);
    static bool IsBuffer(EGpuParamObjectType type);
    static uint32_t GetDataParamSize(EGpuParamDataType type);

public:
    uint32_t getTechniques() const { return static_cast<uint32_t>(mDesc.techniques.length()); }
    TArray<TechniqueType> getCompatibleTechniques() const;
    TArray<TechniqueType> getCompatibleTechniques(const FShaderVariation &variation, bool exact) const;

    const TArray<ShaderVariationParamInfo> getVariationParams() const { return mDesc.variationParams; }

    EQueueSortType getQueueSortType() const { return mDesc.queueSortType; }
    int32_t getQueuePriority() const { return mDesc.queuPriority; }
    bool getAllowSeparablePasses() const { return mDesc.separablePasses; }

    EGpuParamType getParamType(const FString &name) const;
    const FShaderDataParamDesc &getDataParamDesc(const FString &name) const;
    const FShaderObjectParamDesc &getTextureParamDesc(const FString &name) const;
    const FShaderObjectParamDesc &getSamplerParamDesc(const FString &name) const;
    const FShaderObjectParamDesc &getBufferParamDesc(const FString &name) const;

    bool hasDataParam(const FString &name) const;
    bool hasTextureParam(const FString &name) const;
    bool hasSamplerParam(const FString &name) const;
    bool hasBufferParam(const FString &name) const;
    bool hasParamBlock(const FString &name) const;

    const TMap<FString, FShaderDataParamDesc> &getDataParams() const { return mDesc.dataParams; }
    const TMap<FString, FShaderObjectParamDesc> &getTextureParams() const { return mDesc.textureParams; }
    const TMap<FString, FShaderObjectParamDesc> &getBufferParams() const { return mDesc.bufferParams; }
    const TMap<FString, FShaderObjectParamDesc> &getSamplerParams() const { return mDesc.samplerParams; }
    const TMap<FString, FShaderParamBlockDesc> &getParamBlocks() const { return mDesc.paramBlocks; }

    const TArray<FShaderParamAttribute> &getParamAttributes() const { return mDesc.paramAttributes; }

    TextureType getDefaultTexture(uint32_t index) const;
    SamplerStateType getDefaultSampler(uint32_t index) const;
    uint8_t *getDefaultValue(uint32_t index);
    uint32_t getId() const { return mId; }
};