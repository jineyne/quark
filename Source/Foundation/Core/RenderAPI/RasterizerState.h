#pragma once

#include "CorePrerequisites.h"

enum class PolygonMode {
    WireFrame,
    Solid
};

enum class CullingMode {
    None,
    ClockWise,
    CounterClockWise
};

struct DLL_EXPORT RasterizerStateDesc {
public:
    bool operator==(const RasterizerStateDesc &rhs) const;

    bool operator!=(const RasterizerStateDesc &rhs) const;

    PolygonMode polygonMode = PolygonMode::Solid;
    CullingMode cullMode = CullingMode::CounterClockWise;

    float depthBias = 0;
    float depthBiasClamp = 0.0f;
    float slopeScaledDepthBias = 0.0f;
    bool depthClipEnable = true;
    bool scissorEnable = false;
    bool multisampleEnable = true;
    bool antialiasedLineEnable = false;
};

class DLL_EXPORT RasterizerState {
protected:
    RasterizerStateDesc mInfo;

public:
    RasterizerState(const RasterizerStateDesc &info);
    virtual ~RasterizerState() = default;

public:
    static RasterizerState *New(const RasterizerStateDesc &info);
    static RasterizerState *GetDefault();

public:
    const RasterizerStateDesc &getInfo() const;
};

namespace std {
    template<>
    struct hash<RasterizerStateDesc> {
        size_t operator()(const RasterizerStateDesc &info) const {
            size_t hash = 0;
            CombineHash(hash, info.polygonMode);
            CombineHash(hash, info.cullMode);
            CombineHash(hash, info.depthBias);
            CombineHash(hash, info.depthBiasClamp);
            CombineHash(hash, info.slopeScaledDepthBias);
            CombineHash(hash, info.depthClipEnable);
            CombineHash(hash, info.multisampleEnable);
            CombineHash(hash, info.antialiasedLineEnable);
            return hash;
        }
    };
}