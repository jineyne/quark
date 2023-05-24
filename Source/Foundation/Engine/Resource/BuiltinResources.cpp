#include "BuiltinResources.h"

FBuiltinResource::FBuiltinResource() {
    /*FPassDesc passDesc{};
    passDesc.vertexProgramDesc.type = EGpuProgramType::Vertex;
    passDesc.vertexProgramDesc.source = read(FString(RAW_APP_ROOT) + TEXT("/Asset/") + "/ForwardRendering.hlsl");
    passDesc.vertexProgramDesc.entryPoint = TEXT("VSMain");

    passDesc.fragmentProgramDesc.type = EGpuProgramType::Fragment;
    passDesc.fragmentProgramDesc.source = read(path + "/ForwardRendering.hlsl");
    passDesc.fragmentProgramDesc.entryPoint = TEXT("PSMain");

    passDesc.depthStencilStateDesc.stencilEnable = true;
    passDesc.depthStencilStateDesc.depthComparisonFunc = ECompareFunction::Less;

    passDesc.depthStencilStateDesc.depthReadEnable = true;
    passDesc.depthStencilStateDesc.depthWriteEnable = true;
    passDesc.depthStencilStateDesc.frontStencilFailOp = EStencilOperation::Keep;
    passDesc.depthStencilStateDesc.frontStencilZFailOp = EStencilOperation::Increment;
    passDesc.depthStencilStateDesc.backStencilFailOp = EStencilOperation::Keep;
    passDesc.depthStencilStateDesc.backStencilZFailOp = EStencilOperation::Decrement;

    auto pass = FPass::New(passDesc);
    pass->compile();

    auto technique = FTechnique::New("HLSL", {}, FShaderVariation(), { pass });
    FShaderDesc shaderDesc{};
    shaderDesc.techniques = { technique };
    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("Light"), TEXT("Light"), EGpuParamObjectType::StructuredBuffer));

    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("PerObject"), TEXT("PerObject"), EGpuParamObjectType::StructuredBuffer));
    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("PerCall"), TEXT("PerCall"), EGpuParamObjectType::StructuredBuffer));
    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("DiffuseTexture"), TEXT("DiffuseTexture"), EGpuParamObjectType::Texture2D));
    if (gRenderAPI().getName() == TEXT("quark-gl")) {
        shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("LinearRepeatSampler"), TEXT("DiffuseTexture"), EGpuParamObjectType::Sampler2D));
    } else {
        shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("LinearRepeatSampler"), TEXT("LinearRepeatSampler"), EGpuParamObjectType::Sampler2D));
    }

    shaderDesc.addParameter(FShaderDataParamDesc(TEXT("Mat"), TEXT("Mat"), EGpuParamDataType::Struct, 1, sizeof(gMaterialParam)));

    auto shader = FShader::New(TEXT("Default"), shaderDesc);*/
}

FBuiltinResource::~FBuiltinResource() {
    mForwardRenderingShader = nullptr;
}

const FResourceHandle<FShader> &FBuiltinResource::getForwardRenderingShader() {
    return mForwardRenderingShader;
}
