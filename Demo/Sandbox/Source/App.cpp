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

struct F_MaterialParams {
    FColor gTint;
} gMaterialParams;

struct FPerObject {
    FMatrix4 gWorldMat;
    FMatrix4 gViewMat;
    FMatrix4 gProjMat;
} gPerObject;

FMaterial *gMaterial;
FGpuParamsSet *gGpuParamsSet;
FGpuParamBlockBuffer *gPerObjectBuffer;
FGpuParamBlockBuffer *gMaterialParamsBuffer;

FResourceHandle<FTexture> gTexture = nullptr;
FResourceHandle<FMesh> gMesh;

FSamplerState *gSamplerState;

uint32_t gTextureWidth = 2;
uint32_t gTextureHeight = 2;

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

/*void updateTexture(uint32_t width, uint32_t height) {
    if (gTexture) {
        gTexture = nullptr;
    }

    FTextureDesc textureDesc{};
    textureDesc.width = width;
    textureDesc.height = height;
    textureDesc.type = ETextureType::e2D;
    textureDesc.format = EPixelFormat::RGBA8;
    textureDesc.usage = ETextureUsage::Static;

    if (gTexture == nullptr) {
        gTexture = FTexture::New(textureDesc);
    }

    FPixelData *pixelData = FPixelData::New(width, height, 1, EPixelFormat::RGBA8);

    TArray<FColor> colors = { FColor::White, FColor::Black };
    for (auto i = 0; i < width; i++) {
        for (auto j = 0; j < height; j++) {
            pixelData->setColorAt(colors[(j + i % colors.length()) % colors.length()], i, j);
        }
    }
    gTexture->writeData(pixelData);
    gMaterial->setTexture("gTexture", gTexture);
}*/

void loadShader(FString path) {
    FPassDesc passDesc{};
    passDesc.vertexProgramDesc.type = EGpuProgramType::Vertex;
    passDesc.vertexProgramDesc.source = read(path + "/Texture.vs.hlsl");
    passDesc.vertexProgramDesc.entryPoint = TEXT("ColorVertexShader");

    passDesc.fragmentProgramDesc.type = EGpuProgramType::Fragment;
    passDesc.fragmentProgramDesc.source = read(path + "/Texture.ps.hlsl");
    passDesc.fragmentProgramDesc.entryPoint = TEXT("ColorPixelShader");
    auto pass = FPass::New(passDesc);
    pass->compile();

    auto technique = FTechnique::New("HLSL", {}, FShaderVariation(), { pass });
    FShaderDesc shaderDesc{};
    shaderDesc.techniques = { technique };
    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("MaterialParams"), TEXT("MaterialParams"), EGpuParamObjectType::StructuredBuffer));
    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("PerObject"), TEXT("PerObject"), EGpuParamObjectType::StructuredBuffer));
    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("gTexture"), TEXT("gTexture"), EGpuParamObjectType::Texture2D));
    if (gRenderAPI().getName() == TEXT("quark-gl")) {
        shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("gSamplerState"), TEXT("gTexture"), EGpuParamObjectType::Sampler2D));
    } else {
        shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("gSamplerState"), TEXT("gSamplerState"), EGpuParamObjectType::Sampler2D));
    }

    auto shader = FShader::New(TEXT("Default"), shaderDesc);
    gMaterial = FMaterial::New(shader);
    gGpuParamsSet = gMaterial->createParamsSet();

    gPerObjectBuffer = FGpuParamBlockBuffer::New(sizeof(gPerObject));
    gPerObject.gProjMat = FMatrix4::Perspective(FRadian(90), 1280.0f / 720.0f, 0.1f, 1000.f);
    gPerObject.gViewMat = FMatrix4::Translate(FVector3(0.0f, 0.0f, 100.0f));
    gPerObject.gWorldMat = FMatrix4::Rotate(45, FVector3(0.0f, 1.0f, 1.0f));
    gPerObject.gWorldMat = gPerObject.gWorldMat * FMatrix4::Scale(FVector3(0.5f, 0.5f, 0.5f));

    gMaterialParamsBuffer = FGpuParamBlockBuffer::New(sizeof(gMaterialParams));
    gMaterialParams.gTint = FColor::White;

    gMaterial->setSamplerState("gSamplerState", gSamplerState);
    // updateTexture(gTextureWidth, gTextureHeight);
}

void loadMesh() {
    gMesh = gImporter().import<FMesh>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Model\\SM_Primitive_Cube_02.fbx"));
    gTexture = gImporter().import<FTexture>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Texture\\PolygonPrototype_Texture_01.png"));
}

int main() {
    FApplicationStartUpDesc desc{};
    desc.renderAPI = TEXT("quark-dx11");
    desc.importers.add(TEXT("quark-assimp-importer"));
    desc.importers.add(TEXT("quark-freeimg-importer"));

    QCoreApplication::StartUp(desc);

    loadShader("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Shader");
    loadMesh();

    // QCoreApplication::Instance().runMainLoop();

    FSamplerStateDesc samplerStateDesc{};
    samplerStateDesc.minFilter = EFilterOptions::Point;
    samplerStateDesc.magFilter = EFilterOptions::Point;
    samplerStateDesc.mipFilter = EFilterOptions::Point;
    gSamplerState = FSamplerState::New(samplerStateDesc);

    QCoreApplication::Instance().setIsMainLoopRunning(true);
    while (QCoreApplication::Instance().isMainLoopRunning()) {
        QCoreApplication::Instance().calculateFrameStats();

        gTime().update();

        gRenderWindowManager().update();

        gPerObjectBuffer->write(0, &gPerObject, sizeof(gPerObject));
        gMaterialParamsBuffer->write(0, &gMaterialParams, sizeof(gMaterialParams));
        auto pass = gMaterial->getPass();
        gMaterial->updateParamsSet(gGpuParamsSet, 0.0f, true);

        gRenderAPI().setRenderTarget(gCoreApplication().getPrimaryWindow());
        gRenderAPI().clearRenderTarget(EFrameBufferType::Color | EFrameBufferType::Depth | EFrameBufferType::Stencil);

        // rendering

        gRenderAPI().setGraphicsPipeline(pass->getGraphicsPipelineState());

        auto data = gMesh->getVertexData();
        gRenderAPI().setVertexDeclaration(data->vertexDeclaration);
        gRenderAPI().setVertexBuffer(0, {data->getBuffer(0)});
        gRenderAPI().setIndexBuffer(gMesh->getIndexBuffer());

        auto params = gGpuParamsSet->getGpuParams();
        params->setParamBlockBuffer("MaterialParams", gMaterialParamsBuffer);
        params->setParamBlockBuffer("PerObject", gPerObjectBuffer);
        params->setTexture(EGpuProgramType::Fragment, TEXT("gTexture"), gTexture);
        params->setSamplerState(EGpuProgramType::Fragment, TEXT("gSamplerState"), gSamplerState);
        gRenderAPI().setGpuParams(params);

        gRenderAPI().drawIndexed(0, gMesh->getIndexCount(), 0, gMesh->getVertexData()->getBufferCount());
        gRenderAPI().swapBuffer(gCoreApplication().getPrimaryWindow());
    }

    gTexture->destroy();
    delete gMaterial;
    delete gSamplerState;
    delete gGpuParamsSet;

    QCoreApplication::ShutDown();
}