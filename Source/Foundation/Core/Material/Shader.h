#pragma once

#include "CorePrerequisites.h"
#include "Image/Texture.h"
#include "RenderAPI/GpuParamDesc.h"
#include "Technique.h"
#include "Renderer/RendererTypes.h"
#include "ShaderVariation.h"
#include "Shader.g.h"

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

QSTRUCT()
struct FShaderParamBase {
    // GENERATED_BODY()

    QPROPERTY()
    FString name;

    QPROPERTY()
    FString gpuVariableName;

    QPROPERTY()
    FString rendererSemantic;

    QPROPERTY()
    uint32_t defaultValueIdx = static_cast<uint32_t>(-1);

    QPROPERTY()
    uint32_t attribIdx = static_cast<uint32_t>(-1);

    FShaderParamBase() = default;
    FShaderParamBase(const FString &name, const FString &gpuVariableName, FString rendererSemantic = TEXT(""))
            : name(name), gpuVariableName(gpuVariableName), rendererSemantic(rendererSemantic) {}
};

QSTRUCT()
struct FShaderDataParamDesc : public FShaderParamBase {
    // GENERATED_BODY()

    QPROPERTY()
    EGpuParamDataType type = EGpuParamDataType::Float1;

    QPROPERTY()
    uint32_t arraySize = 1;

    QPROPERTY()
    uint32_t elementSize = 0;

    FShaderDataParamDesc() = default;
    FShaderDataParamDesc(const FString &name, const FString &gpuVariableName, EGpuParamDataType type,
                         uint32_t arraySize = 1, uint32_t elementSize = 0)
            : FShaderParamBase(name, gpuVariableName), type(type), arraySize(arraySize), elementSize(elementSize) {}
};

QSTRUCT()
struct FShaderObjectParamDesc : public FShaderParamBase {
    // GENERATED_BODY()

    QPROPERTY()
    EGpuParamObjectType type = EGpuParamObjectType::Texture2D;

    QPROPERTY()
    TArray<FString> gpuVariableNames;

    FShaderObjectParamDesc() = default;
    FShaderObjectParamDesc(const FString & name, const FString & gpuVariableName, EGpuParamObjectType type)
            : FShaderParamBase(name, gpuVariableName), type(type) {
        gpuVariableNames.add(gpuVariableName);
    }
};

QSTRUCT()
struct FShaderParamBlockDesc {
    // GENERATED_BODY()

    QPROPERTY()
    FString name;

    QPROPERTY()
    bool shared;

    QPROPERTY()
    FString rendererSemantic;

    QPROPERTY()
    EBufferUsage usage;
};

QSTRUCT()
struct FShaderParamAttribute {
    // GENERATED_BODY()

    QPROPERTY()
    ShaderParamAttributeType type = ShaderParamAttributeType::SpriteUV;

    QPROPERTY()
    FString value;

    QPROPERTY()
    uint32_t nextParamIdx = static_cast<uint32_t>(-1);
};

QSTRUCT()
struct ShaderVariationParamValue {
    // GENERATED_BODY()

    QPROPERTY()
    FString name;

    QPROPERTY()
    int32_t value = 0;
};

QSTRUCT()
struct ShaderVariationParamInfo {
    // GENERATED_BODY()

    QPROPERTY()
    FString name;

    QPROPERTY()
    FString identifier;

    QPROPERTY()
    bool isInternal = true;

    QPROPERTY()
    TArray<ShaderVariationParamValue> values;
};

QSTRUCT()
class DLL_EXPORT FShaderDesc {
    // GENERATED_BODY()

public:
    using TextureType = FResourceHandle<FTexture>;
    using SamplerStateType = FSamplerState *;
    using TechniqueType = FTechnique *;

public:
    QPROPERTY()
    TArray<TechniqueType> techniques;

    QPROPERTY()
    TArray<ShaderVariationParamInfo> variationParams;

    QPROPERTY()
    TMap<FString, FShaderDataParamDesc> dataParams;

    QPROPERTY()
    TMap<FString, FShaderObjectParamDesc> textureParams;

    QPROPERTY()
    TMap<FString, FShaderObjectParamDesc> bufferParams;

    QPROPERTY()
    TMap<FString, FShaderObjectParamDesc> samplerParams;

    QPROPERTY()
    TMap<FString, FShaderParamBlockDesc> paramBlocks;

    QPROPERTY()
    TArray<uint8_t> dataDefaultValues;

    QPROPERTY()
    TArray<SamplerStateType> samplerDefaultValues;

    QPROPERTY()
    TArray<TextureType> textureDefaultValues;

    QPROPERTY()
    TArray<FShaderParamAttribute> paramAttributes;

    QPROPERTY()
    EQueueSortType queueSortType = EQueueSortType::None;

    QPROPERTY()
    int32_t queuPriority = 0;

    QPROPERTY()
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

QCLASS()
class DLL_EXPORT FShader : public FResource {
    // GENERATED_BODY()

public:
    using TechniqueType = FShaderDesc::TechniqueType;
    using TextureType = FShaderDesc::TextureType;
    using SamplerStateType = FShaderDesc::SamplerStateType;

    static uint32_t mNextShaderId;

protected:
    QPROPERTY()
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
