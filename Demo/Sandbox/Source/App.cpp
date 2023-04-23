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

struct FMaterialParam {
    FColor globalAmbient = FColor::Black;
    FColor ambientColor = FColor::Black;
    FColor emissiveColor = FColor::Black;
    FColor diffuseColor = FColor::Black;
    FColor specularColor = FColor::Black;

    FColor reflectance = FColor::Black;

    float opacity = 1.f;
    float specularPower = 0.5f;
    float indexOfRefraction = 2;
    BOOL hasAmbientTexture = FALSE;

    BOOL hasEmissiveTexture = FALSE;
    BOOL hasDiffuseTexture = FALSE;
    BOOL hasSpecularTexture = FALSE;
    BOOL hasSpecularPowerTexture = FALSE;

    BOOL hasNormalTexture = FALSE;
    BOOL hasBumpTexture = FALSE;
    BOOL hasOpacityTexture = FALSE;
    float bumpIntensity = 1;

    float specularScale = 2;
    float alphaThreshold = 3;
    float padding[2];
} gMaterialParam;

FMaterial *gMaterial;
FGpuParamBlockBuffer *gMaterialBuffer;

FResourceHandle<FTexture> gTexture = nullptr;
FResourceHandle<FMesh> gMesh;

FSamplerState *gSamplerState;
FRenderable *gRenderable;

FTransform *gTransform;
FCameraBase *gMainCamera;
FLightBase *gMainLight;

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
    passDesc.vertexProgramDesc.source = read(path + "/ForwardRendering.hlsl");
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

    auto shader = FShader::New(TEXT("Default"), shaderDesc);
    gMaterial = FMaterial::New(shader);

    gMaterialParam.hasDiffuseTexture = true;
    gMaterial->setStructData(TEXT("Mat"), &gMaterialParam, sizeof(gMaterialParam));

    FSamplerStateDesc samplerStateDesc{};
    samplerStateDesc.minFilter = EFilterOptions::Linear;
    samplerStateDesc.magFilter = EFilterOptions::Linear;
    samplerStateDesc.mipFilter = EFilterOptions::Linear;
    gSamplerState = FSamplerState::New(samplerStateDesc);
    gMaterial->setSamplerState(TEXT("LinearRepeatSampler"), gSamplerState);

    // updateTexture(gTextureWidth, gTextureHeight);
    // gMaterial->setBuffer(TEXT("Light"), );

    gRenderable->setMaterial(gMaterial);
}

void loadMesh() {
    gMesh = gImporter().import<FMesh>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Model\\SM_Primitive_Cube_02.fbx"));
    gTexture = gImporter().import<FTexture>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Texture\\PolygonPrototype_Texture_01.png"));

    gRenderable->setMesh(gMesh.get());
    gMaterial->setTexture(TEXT("DiffuseTexture"), gTexture);
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

    gMainLight = q_new<FLightBase>();
    gMainLight->setIntensity(1);
    gMainLight->setType(ELightType::Directional);
    gMainLight->setColor(FColor::White);
    gMainLight->setTransform(q_new<FTransform>(nullptr));
    gMainLight->getTransform()->rotate(FQuaternion(0, 0, 0));

    gRenderable->initialize();
    gMainCamera->initialize();
    gMainLight->initialize();

    gRenderable->update(EActorDirtyFlags::Transform);

    QCoreApplication::Instance().runMainLoop();

    gTexture->destroy();
    delete gMaterial;
    delete gSamplerState;

    QCoreApplication::ShutDown();
}