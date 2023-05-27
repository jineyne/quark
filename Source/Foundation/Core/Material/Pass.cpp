#include "Pass.h"
#include "RenderAPI/GraphicsPipelineState.h"
#include "Exception/Exception.h"

Pass::Pass() : mGraphicsPipelineState(nullptr)  { }

Pass::Pass(const FPassDesc &desc) : mDesc(desc) { }

Pass::~Pass() {
    if (mGraphicsPipelineState != nullptr) {
        q_delete(mGraphicsPipelineState);
        mGraphicsPipelineState = nullptr;
    }
}

Pass *Pass::New(const FPassDesc &desc) {
    return q_new<Pass>(desc);
}

void Pass::compile() {
    if (mGraphicsPipelineState != nullptr) {
        return;
    }

    createPipelineState();
}

const GpuProgramDesc &Pass::getProgramDesc(EGpuProgramType type) const {
    switch (type) {
        default:
        case EGpuProgramType::Vertex:
            return mDesc.vertexProgramDesc;

        case EGpuProgramType::Fragment:
            return mDesc.fragmentProgramDesc;
    }
}

void Pass::createPipelineState() {
    PipelineStateDesc desc{};

    if (!mDesc.vertexProgramDesc.source.empty()) {
        desc.vertexProgram = GpuProgram::New(mDesc.vertexProgramDesc);
        if (!desc.vertexProgram->isCompiled()) {
            EXCEPT(LogMaterial, InternalErrorException, TEXT("Failed to compile vertex shader: %ls"), *desc.vertexProgram->getCompileErrorMessage());
        }
    }

    if (!mDesc.fragmentProgramDesc.source.empty()) {
        desc.fragmentProgram = GpuProgram::New(mDesc.fragmentProgramDesc);
        if (!desc.fragmentProgram->isCompiled()) {
            EXCEPT(LogMaterial, InternalErrorException, TEXT("Failed to compile fragment shader: %ls"), *desc.fragmentProgram->getCompileErrorMessage());
        }
    }

    desc.depthStencilState = DepthStencilState::New(mDesc.depthStencilStateDesc);

    mGraphicsPipelineState = GraphicsPipelineState::New(desc);
}
