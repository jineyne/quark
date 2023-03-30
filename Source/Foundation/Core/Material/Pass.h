#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/GpuProgram.h"
#include "RenderAPI/GraphicsPipelineState.h"
#include "MaterialType.h"

struct FPassDesc {
    FGpuProgramDesc vertexProgramDesc;
    FGpuProgramDesc fragmentProgramDesc;

    // BlendStateDesc blendStateDesc;
    // RasterizerStateDesc rasterizerStateDesc;
    FDepthStencilStateDesc depthStencilStateDesc;
};

class DLL_EXPORT FPass {
private:
    using GpuProgramPtrType = FGpuProgram*;
    using GraphicsPipelineStateType = FGraphicsPipelineState *;

protected:
    FPassDesc mDesc;
    GraphicsPipelineStateType mGraphicsPipelineState = nullptr;

public:
    FPass();
    FPass(const FPassDesc &desc);

    virtual ~FPass();

public:
    static FPass *New(const FPassDesc &desc);

public:
    void compile();

    const FGpuProgramDesc &getProgramDesc(EGpuProgramType type) const;
    const auto &getGraphicsPipelineState() const { return mGraphicsPipelineState;  }

private:
    void createPipelineState();
};