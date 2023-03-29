#include "Pass.h"
#include "RenderAPI/GraphicsPipelineState.h"
#include "Exception/Exception.h"

FPass::FPass() : mGraphicsPipelineState(nullptr)  { }

FPass::FPass(const FPassDesc &desc) : mDesc(desc) { }

FPass::~FPass() {
    if (mGraphicsPipelineState != nullptr) {
        q_delete(mGraphicsPipelineState);
        mGraphicsPipelineState = nullptr;
    }
}

FPass *FPass::New(const FPassDesc &desc) {
    return q_new<FPass>(desc);
}

void FPass::compile() {
    if (mGraphicsPipelineState != nullptr) {
        return;
    }

    createPipelineState();
}

const FGpuProgramDesc &FPass::getProgramDesc(EGpuProgramType type) const {
    switch (type) {
        default:
        case EGpuProgramType::Vertex:
            return mDesc.vertexProgramDesc;

        case EGpuProgramType::Fragment:
            return mDesc.fragmentProgramDesc;
    }
}

void FPass::createPipelineState() {
    FPipelineStateDesc desc{};

    if (!mDesc.vertexProgramDesc.source.empty()) {
        desc.vertexProgram = FGpuProgram::New(mDesc.vertexProgramDesc);
        if (!desc.vertexProgram->isCompiled()) {
            EXCEPT(FLogMaterial, InternalErrorException, TEXT("Failed to compile vertex shader: %ls"), *desc.vertexProgram->getCompileErrorMessage());
        }
    }

    if (!mDesc.fragmentProgramDesc.source.empty()) {
        desc.fragmentProgram = FGpuProgram::New(mDesc.fragmentProgramDesc);
        if (!desc.fragmentProgram->isCompiled()) {
            EXCEPT(FLogMaterial, InternalErrorException, TEXT("Failed to compile fragment shader: %ls"), *desc.fragmentProgram->getCompileErrorMessage());
        }
    }

    // desc.blendState = BlendState::New(mDesc.blendStateDesc);
    // desc.rasterizerState = RasterizerState::New(mDesc.rasterizerStateDesc);

    mGraphicsPipelineState = FGraphicsPipelineState::New(desc);
}
