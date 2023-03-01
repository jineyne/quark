#pragma once

#include "CorePrerequisites.h"
#include "Image/Color.h"

enum class CompareFunction {
    AlwaysFail,
    AlwaysPass,
    Less,
    LessEqual,
    Equal,
    NotEqual,
    GreaterEqual,
    Greater
};

enum class TextureAddressingMode {
    Wrap,
    Mirror,
    Clamp,
    Border
};

enum FilterType {
    Min,
    Mag,
    Mip,
};

enum class FilterOptions {
    None = 0,
    Point = 1,
    Linear = 2,
    Anisotropic = 3,
};

struct FUVWAddressingMode {
    TextureAddressingMode u, v, w;

    FUVWAddressingMode()
            : u(TextureAddressingMode::Wrap), v(TextureAddressingMode::Wrap), w(TextureAddressingMode::Wrap) {
    }

    bool operator==(const FUVWAddressingMode &rhs) const {
        return u == rhs.u && v == rhs.v && w == rhs.w;
    }
};

struct DLL_EXPORT FSamplerStateDesc {
public:
    FUVWAddressingMode addressMode;

    FilterOptions minFilter = FilterOptions::Linear;
    FilterOptions magFilter = FilterOptions::Linear;
    FilterOptions mipFilter = FilterOptions::Linear;

    uint32_t maxAniso = 0;
    float mipmapBias = 0;

    float mipMin = -std::numeric_limits<float>::max();
    float mipMax = std::numeric_limits<float>::max();

    FColor borderColor = FColor::White;

    CompareFunction comparisonFunc = CompareFunction::AlwaysPass;

public:
    FSamplerStateDesc();
    bool operator==(const FSamplerStateDesc &rhs) const;
};

class DLL_EXPORT FSamplerState {
protected:
    FSamplerStateDesc mDesc;

public:
    FSamplerState(const FSamplerStateDesc &desc);
    virtual ~FSamplerState();

public:
    static FSamplerState *New(const FSamplerStateDesc &desc);
    static FSamplerState *GetDefault();

    static size_t GenerateHash(const FSamplerStateDesc &desc);

public:
    FilterOptions getTextureFiltering(FilterType ft) const;
    const FSamplerStateDesc &getDesc() const { return mDesc; }

protected:
    virtual void createInternal() {}
};

namespace std {
    /**	Hash value generator for SAMPLER_STATE_DESC. */
    template<>
    struct hash<FSamplerStateDesc> {
        size_t operator()(const FSamplerStateDesc &value) const {
            return (size_t) FSamplerState::GenerateHash(value);
        }
    };
}

