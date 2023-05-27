#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/GpuProgram.h"
#include "RenderAPI/GraphicsPipelineState.h"
#include "MaterialType.h"

struct FPassDesc {
    GpuProgramDesc vertexProgramDesc;
    GpuProgramDesc fragmentProgramDesc;

    // BlendStateDesc blendStateDesc;
    // RasterizerStateDesc rasterizerStateDesc;
    DepthStencilStateDesc depthStencilStateDesc;
};

class DLL_EXPORT Pass {
private:
    using GpuProgramPtrType = GpuProgram*;
    using GraphicsPipelineStateType = GraphicsPipelineState *;

protected:
    FPassDesc mDesc;
    GraphicsPipelineStateType mGraphicsPipelineState = nullptr;

public:
    Pass();
    Pass(const FPassDesc &desc);

    virtual ~Pass();

public:
    static Pass *New(const FPassDesc &desc);

public:
    void compile();

    const GpuProgramDesc &getProgramDesc(EGpuProgramType type) const;
    const auto &getGraphicsPipelineState() const { return mGraphicsPipelineState;  }

private:
    void createPipelineState();
};