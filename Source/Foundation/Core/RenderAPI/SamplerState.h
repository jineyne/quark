#pragma once

#include "CorePrerequisites.h"
#include "Image/Color.h"

enum class ETextureAddressingMode {
    Wrap,
    Mirror,
    Clamp,
    Border
};

enum EFilterType {
    Min,
    Mag,
    Mip,
};

enum class EFilterOptions {
    None = 0,
    Point = 1,
    Linear = 2,
    Anisotropic = 3,
};

struct UVWAddressingMode {
    ETextureAddressingMode u, v, w;

    UVWAddressingMode()
            : u(ETextureAddressingMode::Wrap), v(ETextureAddressingMode::Wrap), w(ETextureAddressingMode::Wrap) {
    }

    bool operator==(const UVWAddressingMode &rhs) const {
        return u == rhs.u && v == rhs.v && w == rhs.w;
    }
};

struct DLL_EXPORT SamplerStateDesc {
public:
    UVWAddressingMode addressMode;

    EFilterOptions minFilter = EFilterOptions::Linear;
    EFilterOptions magFilter = EFilterOptions::Linear;
    EFilterOptions mipFilter = EFilterOptions::Linear;

    uint32_t maxAniso = 0;
    float mipmapBias = 0;

    float mipMin = -std::numeric_limits<float>::max();
    float mipMax = std::numeric_limits<float>::max();

    Color borderColor = Color::White;

    ECompareFunction comparisonFunc = ECompareFunction::AlwaysPass;

public:
    SamplerStateDesc();
    bool operator==(const SamplerStateDesc &rhs) const;
};

class DLL_EXPORT SamplerState {
protected:
    SamplerStateDesc mDesc;

public:
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

