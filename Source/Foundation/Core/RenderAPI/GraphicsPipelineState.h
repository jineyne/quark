#pragma once

#include "CorePrerequisites.h"
#include "BlendState.h"
#include "GpuPipelineParamInfo.h"
#include "DepthStencilState.h"

struct PipelineStateDesc {
    DepthStencilState *depthStencilState = nullptr;

    GpuProgram *vertexProgram = nullptr;
    GpuProgram *fragmentProgram = nullptr;

    BlendState *blendState;
};

class DLL_EXPORT GraphicsPipelineState {
private:
    GpuProgram *mVertexProgram = nullptr;
    GpuProgram *mFragmentProgram = nullptr;

    BlendState *mBlendState;
    DepthStencilState *mDepthStencilState = nullptr;

    FGpuPipelineParamInfo *mParamInfo;

public:
    GraphicsPipelineState() = default;
    GraphicsPipelineState(const PipelineStateDesc &desc);

public:
    static GraphicsPipelineState *New(const PipelineStateDesc &desc);

public:
    bool hasVertexProgram() const { return mVertexProgram != nullptr; }
    bool hasFragmentProgram() const { return mFragmentProgram != nullptr; }
    bool hasBlendState() const { return mBlendState != nullptr; }

    auto getVertexProgram() const { return mVertexProgram; }
    auto getFragmentProgram() const { return mFragmentProgram; }

    auto getBlendState() const { return mBlendState; }
    auto getDepthStencilState() const { return mDepthStencilState; }

    FGpuPipelineParamInfo *getParamInfo() const { return mParamInfo; }
};