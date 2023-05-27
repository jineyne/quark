#include "DepthStencilState.h"
#include "Manager/RenderStateManager.h"

bool DepthStencilStateDesc::operator==(const DepthStencilStateDesc &rhs) const {
    return depthReadEnable == rhs.depthReadEnable &&
           depthWriteEnable == rhs.depthWriteEnable &&
           depthComparisonFunc == rhs.depthComparisonFunc &&
           stencilEnable == rhs.stencilEnable &&
           stencilReadMask == rhs.stencilReadMask &&
           stencilWriteMask == rhs.stencilWriteMask &&
           frontStencilFailOp == rhs.frontStencilFailOp &&
           frontStencilZFailOp == rhs.frontStencilZFailOp &&
           frontStencilPassOp == rhs.frontStencilPassOp &&
           frontStencilComparisonFunc == rhs.frontStencilComparisonFunc &&
           backStencilFailOp == rhs.backStencilFailOp &&
           backStencilZFailOp == rhs.backStencilZFailOp &&
           backStencilPassOp == rhs.backStencilPassOp &&
           backStencilComparisonFunc == rhs.backStencilComparisonFunc;
}

DepthStencilState::DepthStencilState(const DepthStencilStateDesc &desc) : mDesc(desc) {}

DepthStencilState::~DepthStencilState() {
    gRenderStateManager().notifyDepthStencilStateDestroyed(mDesc);
}

DepthStencilState *DepthStencilState::New(const DepthStencilStateDesc &desc) {
    return gRenderStateManager().createDepthStencilState(desc);
}

DepthStencilState *DepthStencilState::Default() {
    return gRenderStateManager().getDefaultDepthStencilState();
}

uint64_t DepthStencilState::GenerateHash(const DepthStencilStateDesc &desc) {
    size_t hash = 0;
    CombineHash(hash, desc.depthReadEnable);
    CombineHash(hash, desc.depthWriteEnable);
    CombineHash(hash, (uint32_t) desc.depthComparisonFunc);
    CombineHash(hash, desc.stencilEnable);
    CombineHash(hash, desc.stencilReadMask);
    CombineHash(hash, desc.stencilWriteMask);
    CombineHash(hash, (uint32_t) desc.frontStencilFailOp);
    CombineHash(hash, (uint32_t) desc.frontStencilZFailOp);
    CombineHash(hash, (uint32_t) desc.frontStencilPassOp);
    CombineHash(hash, (uint32_t) desc.frontStencilComparisonFunc);
    CombineHash(hash, (uint32_t) desc.backStencilFailOp);
    CombineHash(hash, (uint32_t) desc.backStencilZFailOp);
    CombineHash(hash, (uint32_t) desc.backStencilPassOp);
    CombineHash(hash, (uint32_t) desc.backStencilComparisonFunc);

    return (uint64_t) hash;
}
