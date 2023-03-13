#pragma once

#include "CorePrerequisites.h"
#include "GpuPipelineParamInfo.h"

struct FPipelineStateDesc {
    FGpuProgram *vertexProgram = nullptr;
    FGpuProgram *fragmentProgram = nullptr;
};

class DLL_EXPORT FGraphicsPipelineState {
private:
    FGpuProgram *mVertexProgram = nullptr;
    FGpuProgram *mFragmentProgram = nullptr;

    FGpuPipelineParamInfo *mParamInfo;

public:
    FGraphicsPipelineState() = default;
    FGraphicsPipelineState(const FPipelineStateDesc &desc);

public:
    static FGraphicsPipelineState *New(const FPipelineStateDesc &desc);

public:
    bool hasVertexProgram() const { return mVertexProgram != nullptr; }
    bool hasFragmentProgram() const { return mFragmentProgram != nullptr; }

    const auto &getVertexProgram() const { return mVertexProgram; }
    const auto &getFragmentProgram() const { return mFragmentProgram; }
    FGpuPipelineParamInfo *getParamInfo() const { return mParamInfo; }
};