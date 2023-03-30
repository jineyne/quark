#pragma once

#include "CorePrerequisites.h"
#include "GpuPipelineParamInfo.h"
#include "DepthStencilState.h"

struct FPipelineStateDesc {
    FDepthStencilState *depthStencilState = nullptr;

    FGpuProgram *vertexProgram = nullptr;
    FGpuProgram *fragmentProgram = nullptr;
};

class DLL_EXPORT FGraphicsPipelineState {
private:
    FGpuProgram *mVertexProgram = nullptr;
    FGpuProgram *mFragmentProgram = nullptr;

    FDepthStencilState *mDepthStencilState = nullptr;

    FGpuPipelineParamInfo *mParamInfo;

public:
    FGraphicsPipelineState() = default;
    FGraphicsPipelineState(const FPipelineStateDesc &desc);

public:
    static FGraphicsPipelineState *New(const FPipelineStateDesc &desc);

public:
    bool hasVertexProgram() const { return mVertexProgram != nullptr; }
    bool hasFragmentProgram() const { return mFragmentProgram != nullptr; }

    auto getVertexProgram() const { return mVertexProgram; }
    auto getFragmentProgram() const { return mFragmentProgram; }

    auto getDepthStencilState() const { return mDepthStencilState; }

    FGpuPipelineParamInfo *getParamInfo() const { return mParamInfo; }
};