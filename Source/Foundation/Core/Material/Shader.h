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

enum class EShaderParamAttributeType {
    SpriteUV,
    Name,
    HDR
};

QSTRUCT()
struct DLL_EXPORT ShaderParamBase {
    GENERATED_BODY()

    QPROPERTY()
    String name;

    QPROPERTY()
    String gpuVariableName;

    QPROPERTY()
    String rendererSemantic;

    QPROPERTY()
    uint32_t defaultValueIdx = static_cast<uint32_t>(-1);

    QPROPERTY()
    uint32_t attribIdx = static_cast<uint32_t>(-1);

    ShaderParamBase() = default;
    ShaderParamBase(const String &name, const String &gpuVariableName, String rendererSemantic = TEXT(""));
};

QSTRUCT()
struct DLL_EXPORT ShaderDataParamDesc : public ShaderParamBase {
    GENERATED_BODY()

    QPROPERTY()
    EGpuParamDataType type = EGpuParamDataType::Float1;

    QPROPERTY()
    uint32_t arraySize = 1;

    QPROPERTY()
    uint32_t elementSize = 0;

    ShaderDataParamDesc() = default;
    ShaderDataParamDesc(const String &name, const String &gpuVariableName, EGpuParamDataType type,
                        uint32_t arraySize = 1, uint32_t elementSize = 0);
};

QSTRUCT()
struct DLL_EXPORT ShaderObjectParamDesc : public ShaderParamBase {
    GENERATED_BODY()

    QPROPERTY()
    EGpuParamObjectType type = EGpuParamObjectType::Texture2D;

    QPROPERTY()
    TArray<String> gpuVariableNames;

    ShaderObjectParamDesc() = default;
    ShaderObjectParamDesc(const String & name, const String & gpuVariableName, EGpuParamObjectType type);
};

QSTRUCT()
struct DLL_EXPORT ShaderParamBlockDesc {
    GENERATED_BODY()

    QPROPERTY()
    String name;

    QPROPERTY()
    bool shared;

    QPROPERTY()
    String rendererSemantic;

    QPROPERTY()
    EBufferUsage usage;
};

QSTRUCT()
struct DLL_EXPORT ShaderParamAttribute {
    GENERATED_BODY()

    QPROPERTY()
    EShaderParamAttributeType type = EShaderParamAttributeType::SpriteUV;

    QPROPERTY()
    String value;

    QPROPERTY()
    uint32_t nextParamIdx = static_cast<uint32_t>(-1);
};

QSTRUCT()
struct DLL_EXPORT ShaderVariationParamValue {
    GENERATED_BODY()

    QPROPERTY()
    String name;

    QPROPERTY()
    int32_t value = 0;
};

QSTRUCT()
struct DLL_EXPORT ShaderVariationParamInfo {
    GENERATED_BODY()

    QPROPERTY()
    String name;

    QPROPERTY()
    String identifier;

    QPROPERTY()
    bool isInternal = true;

    QPROPERTY()
    TArray<ShaderVariationParamValue> values;
};

QSTRUCT()
struct DLL_EXPORT ShaderDesc {
    GENERATED_BODY()

public:
    using TextureType = FResourceHandle<Texture>;
    using SamplerStateType = SamplerState *;
    using TechniqueType = Technique *;

public:
    TArray<TechniqueType> techniques;
    TArray<ShaderVariationParamInfo> variationParams;
    TMap<String, ShaderDataParamDesc> dataParams;
    TMap<String, ShaderObjectParamDesc> textureParams;
    TMap<String, ShaderObjectParamDesc> bufferParams;
    TMap<String, ShaderObjectParamDesc> samplerParams;
    TMap<String, ShaderParamBlockDesc> paramBlocks;
    TArray<uint8_t> dataDefaultValues;
    TArray<SamplerStateType> samplerDefaultValues;
    TArray<TextureType> textureDefaultValues;
    TArray<ShaderParamAttribute> paramAttributes;
    EQueueSortType queueSortType = EQueueSortType::None;
    int32_t queuPriority = 0;

    QPROPERTY()
    bool separablePasses = false;

public:
    ShaderDesc();

public:
    void addParameter(ShaderDataParamDesc paramDesc, uint32_t *defaultValue = nullptr);
    void addParameter(ShaderObjectParamDesc paramDesc);
    void addParameter(ShaderObjectParamDesc paramDesc, const SamplerStateType &defaultValue);
    void addParameter(ShaderObjectParamDesc paramDesc, const TextureType &defaultValue);
    void addParameterAttribute(const String &name, const ShaderParamAttribute &attrib);
    void setParamBlockAttribs(const String &name, bool shared, EBufferUsage usage, String rendererSemantic);

private:
    void addParameterInternal(ShaderObjectParamDesc paramDesc, uint32_t defaultValueIndex);
};

QCLASS()
class DLL_EXPORT Shader : public Resource {
    GENERATED_BODY()

public:
    using TechniqueType = ShaderDesc::TechniqueType;
    using TextureType = ShaderDesc::TextureType;
    using SamplerStateType = ShaderDesc::SamplerStateType;

    static uint32_t mNextShaderId;

protected:
    QPROPERTY()
    ShaderDesc mDesc;

    uint32_t mId = -1;

public:
    Shader() = default;
    Shader(uint32_t id);
    Shader(const String &name, const ShaderDesc &desc, uint32_t id);

    virtual ~Shader() = default;

public:
    static Shader *New(const String &name, const ShaderDesc &desc);

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

    EGpuParamType getParamType(const String &name) const;
    const ShaderDataParamDesc &getDataParamDesc(const String &name) const;
    const ShaderObjectParamDesc &getTextureParamDesc(const String &name) const;
    const ShaderObjectParamDesc &getSamplerParamDesc(const String &name) const;
    const ShaderObjectParamDesc &getBufferParamDesc(const String &name) const;

    bool hasDataParam(const String &name) const;
    bool hasTextureParam(const String &name) const;
    bool hasSamplerParam(const String &name) const;
    bool hasBufferParam(const String &name) const;
    bool hasParamBlock(const String &name) const;

    const TMap<String, ShaderDataParamDesc> &getDataParams() const { return mDesc.dataParams; }
    const TMap<String, ShaderObjectParamDesc> &getTextureParams() const { return mDesc.textureParams; }
    const TMap<String, ShaderObjectParamDesc> &getBufferParams() const { return mDesc.bufferParams; }
    const TMap<String, ShaderObjectParamDesc> &getSamplerParams() const { return mDesc.samplerParams; }
    const TMap<String, ShaderParamBlockDesc> &getParamBlocks() const { return mDesc.paramBlocks; }

    const TArray<ShaderParamAttribute> &getParamAttributes() const { return mDesc.paramAttributes; }

    TextureType getDefaultTexture(uint32_t index) const;
    SamplerStateType getDefaultSampler(uint32_t index) const;
    uint8_t *getDefaultValue(uint32_t index);
    uint32_t getId() const { return mId; }
};
