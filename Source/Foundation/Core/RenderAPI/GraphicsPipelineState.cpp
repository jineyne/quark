#include "GraphicsPipelineState.h"

FGraphicsPipelineState::FGraphicsPipelineState(const FPipelineStateDesc &desc)
        : mVertexProgram(desc.vertexProgram), mFragmentProgram(desc.fragmentProgram) {
    FGpuPipelineParamsDesc paramDesc{};
    if (desc.vertexProgram != nullptr) {
        paramDesc.vertexParams = desc.vertexProgram->getParamDesc();
    }

    if (desc.fragmentProgram != nullptr) {
        paramDesc.fragmentParams = desc.fragmentProgram->getParamDesc();
    }

    mParamInfo = FGpuPipelineParamInfo::New(paramDesc);
}

FGraphicsPipelineState *FGraphicsPipelineState::New(const FPipelineStateDesc &desc) {
    return q_new<FGraphicsPipelineState>(desc);
}
