#include "SandboxResourceUtil.h"
#include "RenderAPI/RenderAPI.h"
#include "FileSystem/FileSystem.h"
#include "Importer/Importer.h"
#include "RenderAPI/SamplerState.h"

MaterialParam gMaterialParam;

Shader *SandboxResourceUtil::ForwardRendering = nullptr;
Material *SandboxResourceUtil::RedShipMaterial = nullptr;
Material *SandboxResourceUtil::BlueShipMaterial = nullptr;
Material *SandboxResourceUtil::BulletMaterial = nullptr;

FResourceHandle<Mesh> SandboxResourceUtil::SparrowMesh = nullptr;

void SandboxResourceUtil::Initialize() {
    FPassDesc passDesc{};
    passDesc.vertexProgramDesc.type = EGpuProgramType::Vertex;
    passDesc.vertexProgramDesc.source = Read(Path::Combine(ANSI_TO_TCHAR(RAW_APP_ROOT), TEXT("Asset/Shader/ForwardRendering.hlsl")));
    passDesc.vertexProgramDesc.entryPoint = TEXT("VSMain");

    passDesc.fragmentProgramDesc.type = EGpuProgramType::Fragment;
    passDesc.fragmentProgramDesc.source = Read(Path::Combine(ANSI_TO_TCHAR(RAW_APP_ROOT), TEXT("Asset/Shader/ForwardRendering.hlsl")));
    passDesc.fragmentProgramDesc.entryPoint = TEXT("PSMain");

    passDesc.depthStencilStateDesc.stencilEnable = true;
    passDesc.depthStencilStateDesc.depthComparisonFunc = ECompareFunction::Less;

    passDesc.depthStencilStateDesc.depthReadEnable = true;
    passDesc.depthStencilStateDesc.depthWriteEnable = true;
    passDesc.depthStencilStateDesc.frontStencilFailOp = EStencilOperation::Keep;
    passDesc.depthStencilStateDesc.frontStencilZFailOp = EStencilOperation::Increment;
    passDesc.depthStencilStateDesc.backStencilFailOp = EStencilOperation::Keep;
    passDesc.depthStencilStateDesc.backStencilZFailOp = EStencilOperation::Decrement;

    auto pass = Pass::New(passDesc);
    pass->compile();

    auto technique = Technique::New("HLSL", {}, FShaderVariation(), {pass });
    ShaderDesc shaderDesc{};
    shaderDesc.techniques = { technique };
    shaderDesc.addParameter(ShaderObjectParamDesc(TEXT("Light"), TEXT("Light"), EGpuParamObjectType::StructuredBuffer));

    shaderDesc.addParameter(ShaderObjectParamDesc(TEXT("PerObject"), TEXT("PerObject"), EGpuParamObjectType::StructuredBuffer));
    shaderDesc.addParameter(ShaderObjectParamDesc(TEXT("PerCall"), TEXT("PerCall"), EGpuParamObjectType::StructuredBuffer));
    shaderDesc.addParameter(ShaderObjectParamDesc(TEXT("DiffuseTexture"), TEXT("DiffuseTexture"), EGpuParamObjectType::Texture2D));
    if (gRenderAPI().getName() == TEXT("quark-gl")) {
        shaderDesc.addParameter(ShaderObjectParamDesc(TEXT("LinearRepeatSampler"), TEXT("DiffuseTexture"), EGpuParamObjectType::Sampler2D));
    } else {
        shaderDesc.addParameter(ShaderObjectParamDesc(TEXT("LinearRepeatSampler"), TEXT("LinearRepeatSampler"), EGpuParamObjectType::Sampler2D));
    }

    shaderDesc.addParameter(ShaderDataParamDesc(TEXT("Mat"), TEXT("Mat"), EGpuParamDataType::Struct, 1, sizeof(gMaterialParam)));

    ForwardRendering = Shader::New(TEXT("Default"), shaderDesc);
    RedShipMaterial = Material::New(ForwardRendering);
    BlueShipMaterial = Material::New(ForwardRendering);
    BulletMaterial = Material::New(ForwardRendering);

    gMaterialParam.hasDiffuseTexture = true;
    RedShipMaterial->setStructData(TEXT("Mat"), &gMaterialParam, sizeof(gMaterialParam));
    BlueShipMaterial->setStructData(TEXT("Mat"), &gMaterialParam, sizeof(gMaterialParam));
    BulletMaterial->setStructData(TEXT("Mat"), &gMaterialParam, sizeof(gMaterialParam));

    RedShipMaterial->setTexture(TEXT("DiffuseTexture"), gImporter().import<Texture>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Texture\\StarSparrow_Red.png")));
    BlueShipMaterial->setTexture(TEXT("DiffuseTexture"), gImporter().import<Texture>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Texture\\StarSparrow_Blue.png")));
    BulletMaterial->setTexture(TEXT("DiffuseTexture"), gImporter().import<Texture>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Texture\\PolygonPrototype_Texture_01.png")));

    RedShipMaterial->setSamplerState(TEXT("LinearRepeatSampler"), SamplerState::GetDefault());
    BlueShipMaterial->setSamplerState(TEXT("LinearRepeatSampler"), SamplerState::GetDefault());
    BulletMaterial->setSamplerState(TEXT("LinearRepeatSampler"), SamplerState::GetDefault());

    SparrowMesh =  gImporter().import<Mesh>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Model\\StarSparrow01.fbx"));
}

void SandboxResourceUtil::Finalization() {

}

String SandboxResourceUtil::Read(Path path) {
    auto file = FileSystem::OpenFile(path);
    auto size = file->size();
    char *buf = (char *) malloc(size + 1);
    file->read(buf, size);
    buf[size] = '\0';

    String result = buf;
    delete buf;

    return result;
}

Shader *SandboxResourceUtil::LoadShader(Path path) {
    return nullptr;
}

