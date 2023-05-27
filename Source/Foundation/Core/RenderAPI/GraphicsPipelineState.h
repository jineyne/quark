#pragma once

#include "CorePrerequisites.h"
#include "GpuPipelineParamInfo.h"
#include "DepthStencilState.h"

struct PipelineStateDesc {
    DepthStencilState *depthStencilState = nullptr;

    GpuProgram *vertexProgram = nullptr;
    GpuProgram *fragmentProgram = nullptr;
};

class DLL_EXPORT GraphicsPipelineState {
private:
    GpuProgram *mVertexProgram = nullptr;
    GpuProgram *mFragmentProgram = nullptr;

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

    auto getVertexProgram() const { return mVertexProgram; }
    auto getFragmentProgram() const { return mFragmentProgram; }

    auto getDepthStencilState() const { return mDepthStencilState; }

    FGpuPipelineParamInfo *getParamInfo() const { return mParamInfo; }
};