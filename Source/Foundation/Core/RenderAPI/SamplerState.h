#pragma once

#include "CorePrerequisites.h"
#include "Image/Color.h"
#include "SamplerState.g.h"

QENUM()
enum class ETextureAddressingMode {
    Wrap                QENTRY(),
    Mirror              QENTRY(),
    Clamp               QENTRY(),
    Border              QENTRY()
};

QENUM()
enum EFilterType {
    Min                 QENTRY(),
    Mag                 QENTRY(),
    Mip                 QENTRY(),
};

QENUM()
enum class EFilterOptions {
    None = 0            QENTRY(),
    Point = 1           QENTRY(),
    Linear = 2          QENTRY(),
    Anisotropic = 3     QENTRY(),
};

QSTRUCT()
struct DLL_EXPORT UVWAddressingMode {
    GENERATED_BODY()

    QPROPERTY()
    ETextureAddressingMode u;

    QPROPERTY()
    ETextureAddressingMode v;

    QPROPERTY()
    ETextureAddressingMode w;

    UVWAddressingMode();

    bool operator==(const UVWAddressingMode &rhs) const;
};

QSTRUCT()
struct DLL_EXPORT SamplerStateDesc {
    GENERATED_BODY()

public:
    QPROPERTY()
    UVWAddressingMode addressMode;

    QPROPERTY()
    EFilterOptions minFilter = EFilterOptions::Linear;

    QPROPERTY()
    EFilterOptions magFilter = EFilterOptions::Linear;

    QPROPERTY()
    EFilterOptions mipFilter = EFilterOptions::Linear;

    QPROPERTY()
    uint32_t maxAniso = 0;

    QPROPERTY()
    float mipmapBias = 0;

    QPROPERTY()
    float mipMin = -std::numeric_limits<float>::max();

    QPROPERTY()
    float mipMax = std::numeric_limits<float>::max();

    QPROPERTY()
    Color borderColor = Color::White;

    QPROPERTY()
    ECompareFunction comparisonFunc = ECompareFunction::AlwaysPass;

public:
    SamplerStateDesc();
    bool operator==(const SamplerStateDesc &rhs) const;
};

QCLASS()
class DLL_EXPORT SamplerState : public Object {
public:
    GENERATED_BODY()

protected:
    QPROPERTY()
    SamplerStateDesc mDesc;

public:
    SamplerState() = default;

    SamplerState(const SamplerStateDesc &desc);
    virtual ~SamplerState();

public:
    static SamplerState *New(const SamplerStateDesc &desc);
    static SamplerState *GetDefault();

    static size_t GenerateHash(const SamplerStateDesc &desc);

public:
    EFilterOptions getTextureFiltering(EFilterType ft) const;
    const SamplerStateDesc &getDesc() const { return mDesc; }

protected:
    virtual void createInternal() {}
};

namespace std {
    /**	Hash value generator for SAMPLER_STATE_DESC. */
    template<>
    struct hash<SamplerStateDesc> {
        size_t operator()(const SamplerStateDesc &value) const {
            return (size_t) SamplerState::GenerateHash(value);
        }
    };
}

