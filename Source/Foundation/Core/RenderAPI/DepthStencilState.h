#pragma once

#include "CorePrerequisites.h"

struct DLL_EXPORT DepthStencilStateDesc {
    bool depthReadEnable = true;
    bool depthWriteEnable = true;
    ECompareFunction depthComparisonFunc = ECompareFunction::Less;
    bool stencilEnable = false;
    uint8_t stencilReadMask = 0xff;
    uint8_t stencilWriteMask = 0xff;

    EStencilOperation frontStencilFailOp = EStencilOperation::Keep;
    EStencilOperation frontStencilZFailOp = EStencilOperation::Keep;
    EStencilOperation frontStencilPassOp = EStencilOperation::Keep;
    ECompareFunction frontStencilComparisonFunc = ECompareFunction::AlwaysPass;

    EStencilOperation backStencilFailOp = EStencilOperation::Keep;
    EStencilOperation backStencilZFailOp = EStencilOperation::Keep;
    EStencilOperation backStencilPassOp = EStencilOperation::Keep;
    ECompareFunction backStencilComparisonFunc = ECompareFunction::AlwaysPass;

    bool operator==(const DepthStencilStateDesc &rhs) const;
};

class DLL_EXPORT DepthStencilState {
private:
    DepthStencilStateDesc mDesc;
    uint64_t mHash;

public:
    DepthStencilState(const DepthStencilStateDesc &desc);

    virtual ~DepthStencilState();

public:
    static DepthStencilState *New(const DepthStencilStateDesc &desc);
    static DepthStencilState *Default();

    static uint64_t GenerateHash(const DepthStencilStateDesc &desc);

public:
    uint64_t getHash() const { return mHash; }

    bool getDepthReadEnable() const { return mDesc.depthReadEnable; }
    bool getDepthWriteEnable() const { return mDesc.depthWriteEnable; }
    ECompareFunction getDepthComparisonFunc() const { return mDesc.depthComparisonFunc; }
    bool getStencilEnable() const { return mDesc.stencilEnable; }
    uint8_t getStencilReadMask() const { return mDesc.stencilReadMask; }
    uint8_t getStencilWriteMask() const { return mDesc.stencilWriteMask; }
    EStencilOperation getStencilFrontFailOp() const { return mDesc.frontStencilFailOp; }
    EStencilOperation getStencilFrontZFailOp() const { return mDesc.frontStencilZFailOp; }
    EStencilOperation getStencilFrontPassOp() const { return mDesc.frontStencilPassOp; }
    ECompareFunction getStencilFrontCompFunc() const { return mDesc.frontStencilComparisonFunc; }
    EStencilOperation getStencilBackFailOp() const { return mDesc.backStencilFailOp; }
    EStencilOperation getStencilBackZFailOp() const { return mDesc.backStencilZFailOp; }
    EStencilOperation getStencilBackPassOp() const { return mDesc.backStencilPassOp; }
    ECompareFunction getStencilBackCompFunc() const { return mDesc.backStencilComparisonFunc; }

protected:
    virtual void createInternal() { }
};

namespace std {
    /**	Hash value generator for SAMPLER_STATE_DESC. */
    template<>
    struct hash<DepthStencilStateDesc> {
        size_t operator()(const DepthStencilStateDesc &value) const {
            return (size_t) DepthStencilState::GenerateHash(value);
        }
    };
}

