#pragma once

#include "CorePrerequisites.h"

struct DLL_EXPORT RenderTargetBlendStateDesc {
public:
    bool blendEnable = false;

public:
    bool operator==(const RenderTargetBlendStateDesc &rhs) const;
};

struct DLL_EXPORT BlendStateDesc {
public:
    RenderTargetBlendStateDesc renderTargetDesc;

public:
    static size_t GenerateHash(const BlendStateDesc &desc);

public:
    bool operator==(const BlendStateDesc &rhs) const;
};

class DLL_EXPORT BlendState {
private:
    BlendStateDesc mDesc;

public:
    BlendState(const BlendStateDesc &desc);
    ~BlendState();

public:
    static BlendState *New(const BlendStateDesc &desc);
    static BlendState *GetDefault();

public:
    bool getBlendEnabled() const { return mDesc.renderTargetDesc.blendEnable; }

    friend class RenderStateManager;
};

namespace std {
    /**	Hash value generator for BLEND_STATE_DESC. */
    template<>
    struct hash<BlendStateDesc> {
        size_t operator()(const BlendStateDesc &value) const {
            return (size_t)BlendStateDesc::GenerateHash(value);
        }
    };
}