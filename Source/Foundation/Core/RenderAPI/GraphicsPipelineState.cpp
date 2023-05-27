#include "GraphicsPipelineState.h"

GraphicsPipelineState::GraphicsPipelineState(const PipelineStateDesc &desc)
        : mVertexProgram(desc.vertexProgram), mFragmentProgram(desc.fragmentProgram)
        , mDepthStencilState(desc.depthStencilState) {
    FGpuPipelineParamsDesc paramDesc{};
    if (desc.vertexProgram != nullptr) {
        paramDesc.vertexParams = desc.vertexProgram->getParamDesc();
    }

    if (desc.fragmentProgram != nullptr) {
        paramDesc.fragmentParams = desc.fragmentProgram->getParamDesc();
    }

    mParamInfo = FGpuPipelineParamInfo::New(paramDesc);
}

GraphicsPipelineState *GraphicsPipelineState::New(const PipelineStateDesc &desc) {
    return q_new<GraphicsPipelineState>(desc);
}
