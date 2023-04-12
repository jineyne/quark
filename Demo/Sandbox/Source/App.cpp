#include "CoreApplication.h"

#include "RenderAPI/VertexBuffer.h"
#include "RenderAPI/GraphicsPipelineState.h"
#include "RenderAPI/GpuParams.h"
#include "RenderAPI/GpuParamBlockBuffer.h"
#include "RenderAPI/GpuProgram.h"
#include "RenderAPI/RenderAPI.h"
#include "RenderAPI/IndexBuffer.h"
#include "RenderAPI/VertexDataDesc.h"
#include "Resource/Resources.h"
#include "FileSystem/FileSystem.h"
#include "Math/Radian.h"
#include "Math/Vector3.h"
#include "Misc/Time.h"
#include "Manager/RenderWindowManager.h"
#include "RenderAPI/SamplerState.h"
#include "Mesh/Mesh.h"
#include "Importer/Importer.h"
#include "Material/Material.h"
#include "Renderer/Renderable.h"
#include "Renderer/Renderer.h"

struct FLightParamDef {
    FColor ambientColor;
    FColor diffuseColor;
    FVector3 lightDirection;
    float padding;
} gLightParam;

FMaterial *gMaterial;
FGpuParamBlockBuffer *gLightBuffer;

FResourceHandle<FTexture> gTexture = nullptr;
FResourceHandle<FMesh> gMesh;

FSamplerState *gSamplerState;
FRenderable *gRenderable;

FTransform *gTransform;
FCameraBase *gMainCamera;

FString read(FString path) {
    auto file = FFileSystem::OpenFile(path);
    auto size = file->size();
    char *buf = (char *) malloc(size + 1);
    file->read(buf, size);
    buf[size] = '\0';

    FString result = buf;
    delete buf;

    return result;
}

void loadShader(FString path) {
    FPassDesc passDesc{};
    passDesc.vertexProgramDesc.type = EGpuProgramType::Vertex;
    passDesc.vertexProgramDesc.source = read(path + "/Texture.vs.hlsl");
    passDesc.vertexProgramDesc.entryPoint = TEXT("main");

    passDesc.fragmentProgramDesc.type = EGpuProgramType::Fragment;
    passDesc.fragmentProgramDesc.source = read(path + "/Texture.ps.hlsl");
    passDesc.fragmentProgramDesc.entryPoint = TEXT("main");

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
    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("MaterialParams"), TEXT("MaterialParams"), EGpuParamObjectType::StructuredBuffer));
    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("Light"), TEXT("Light"), EGpuParamObjectType::StructuredBuffer));
    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("PerObject"), TEXT("PerObject"), EGpuParamObjectType::StructuredBuffer));
    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("gTexture"), TEXT("gTexture"), EGpuParamObjectType::Texture2D));
    if (gRenderAPI().getName() == TEXT("quark-gl")) {
        shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("gSamplerState"), TEXT("gTexture"), EGpuParamObjectType::Sampler2D));
    } else {
        shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("gSamplerState"), TEXT("gSamplerState"), EGpuParamObjectType::Sampler2D));
    }

    auto shader = FShader::New(TEXT("Default"), shaderDesc);
    gMaterial = FMaterial::New(shader);

    gLightBuffer = FGpuParamBlockBuffer::New(sizeof(gLightParam));
    gLightParam.ambientColor = FColor(0.15f, 0.15f, 0.15f);
    gLightParam.diffuseColor = FColor::White;
    gLightParam.lightDirection = FVector3(0.0f, 0.0f, 1.0f);
    gLightParam.padding = 0.0f;

    FSamplerStateDesc samplerStateDesc{};
    samplerStateDesc.minFilter = EFilterOptions::Point;
    samplerStateDesc.magFilter = EFilterOptions::Point;
    samplerStateDesc.mipFilter = EFilterOptions::Point;
    gSamplerState = FSamplerState::New(samplerStateDesc);
    gMaterial->setSamplerState(TEXT("gSamplerState"), gSamplerState);

    // updateTexture(gTextureWidth, gTextureHeight);
    // gMaterial->setBuffer(TEXT("Light"), );

    gRenderable->setMaterial(gMaterial);
}

void loadMesh() {
    gMesh = gImporter().import<FMesh>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Model\\SM_Primitive_Cube_02.fbx"));
    gTexture = gImporter().import<FTexture>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Texture\\PolygonPrototype_Texture_01.png"));

    gRenderable->setMesh(gMesh.get());
    gMaterial->setTexture(TEXT("gTexture"), gTexture);
}

double value = 0;

int main() {
    FApplicationStartUpDesc desc{};
    desc.renderAPI = TEXT("quark-dx11");
    desc.importers.add(TEXT("quark-assimp-importer"));
    desc.importers.add(TEXT("quark-freeimg-importer"));

    QCoreApplication::StartUp(desc);

    gTransform = q_new<FTransform>(nullptr);
    gTransform->setPosition(FVector3(0.0f, 0.0f, 100.0f));
    gTransform->setScale(FVector3(0.5f, 0.5f, 0.5f));

    gRenderable = q_new<FRenderable>();
    gRenderable->setTransform(gTransform);

    gMainCamera = q_new<FCameraBase>();
    gMainCamera->setMain(true);
    gMainCamera->setAspectRatio(1280.0f / 720.0f);
    gMainCamera->setHorzFov(FRadian(60));
    gMainCamera->setNearClipDistance(0.1f);
    gMainCamera->setFarClipDistance(1000.0f);
    gMainCamera->setTransform(q_new<FTransform>(nullptr));
    gMainCamera->getViewport()->setTarget(gCoreApplication().getPrimaryWindow());
    gMainCamera->getViewport()->setClearFlags(EClearFlags::Color | EClearFlags::Depth | EClearFlags::Stencil);

    loadShader("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Shader");
    loadMesh();

    gRenderable->initialize();
    gMainCamera->initialize();

    // QCoreApplication::Instance().runMainLoop();

    gRenderable->update(EActorDirtyFlags::Transform);

    QCoreApplication::Instance().runMainLoop();

    gTexture->destroy();
    delete gMaterial;
    delete gSamplerState;

    QCoreApplication::ShutDown();
}