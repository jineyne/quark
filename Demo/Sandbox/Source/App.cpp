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

struct FMaterialParams {
    FColor gTint;
} gMaterialParams;

struct FPerObject {
    FMatrix4 gWorldMat;
    FMatrix4 gViewMat;
    FMatrix4 gProjMat;
} gPerObject;

float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.5f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
};

float positions[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
};

float texCoord[] = {
        0.0f, 1.0f,
        0.5f, 0.0f,
        1.0f, 1.0f,
};

uint32_t indices[] = {
        0, 1, 2,
};

FGraphicsPipelineState *gPipeline;
FVertexDeclaration *gVertexDeclaration;
FVertexBuffer *gVBO;
FIndexBuffer *gIBO;
FMesh *gMesh;
FGpuParams *gGpuParams;
FGpuParamBlockBuffer *gPerObjectBuffer;
FGpuParamBlockBuffer *gMaterialParamsBuffer;

FSamplerState *gSamplerState;

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
    FPipelineStateDesc pipelineStateDesc{};

    FGpuProgramDesc vd{};
    vd.type = EGpuProgramType::Vertex;
    vd.source = read(path + "/Texture.vs.hlsl");
    vd.entryPoint = TEXT("ColorVertexShader");
    pipelineStateDesc.vertexProgram = FGpuProgram::New(vd);

    FGpuProgramDesc fd{};
    fd.type = EGpuProgramType::Fragment;
    fd.source = read(path + "/Texture.ps.hlsl");
    fd.entryPoint = TEXT("ColorPixelShader");
    pipelineStateDesc.fragmentProgram = FGpuProgram::New(fd);

    gPipeline = FGraphicsPipelineState::New(pipelineStateDesc);

    gPerObjectBuffer = FGpuParamBlockBuffer::New(sizeof(gPerObject));
    gPerObject.gProjMat = FMatrix4::Perspective(FRadian(90), 800.0f / 600.0f, 0.1f, 1000.f);
    gPerObject.gViewMat = FMatrix4::Translate(FVector3(0.0f, 0.0f, 5.0f));
    gPerObject.gWorldMat = FMatrix4::Identity();

    gMaterialParamsBuffer = FGpuParamBlockBuffer::New(sizeof(gMaterialParams));
    gMaterialParams.gTint = FColor::White;

    gGpuParams = FGpuParams::New(gPipeline);
}

int main() {
    FApplicationStartUpDesc desc{};
    desc.renderAPI = TEXT("quark-dx11");

    QCoreApplication::StartUp(desc);

    loadShader("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Shader");


    auto vdd = FVertexDataDesc::New();
    vdd->addElement(EVertexElementType::Float3, EVertexElementSemantic::Position);
    vdd->addElement(EVertexElementType::Float2, EVertexElementSemantic::TexCoord);


    gVertexDeclaration = FVertexDeclaration::New(vdd);

    auto md = FMeshData::New(3, 3, vdd);
    md->setVertexData(EVertexElementSemantic::Position, positions, sizeof(positions));
    md->setVertexData(EVertexElementSemantic::TexCoord, texCoord, sizeof(texCoord));
    auto indexData = md->getIndex32();
    std::memcpy(indexData, indices, sizeof(indices));

    gMesh = FMesh::New(md, FMeshDesc::Default);

    FVertexBufferDesc vd{};
    vd.usage = EBufferUsage::Static;
    vd.vertexSize = gVertexDeclaration->getVertexSize();
    vd.vertexCount = 3;
    gVBO = FVertexBuffer::New(vd);
    gVBO->writeData(0, sizeof(vertices), vertices);

    FIndexBufferDesc id{};
    id.usage = EBufferUsage::Static;
    id.indexType = EIndexType::_32bit;
    id.indexCount = 3;
    gIBO = FIndexBuffer::New(id);
    gIBO->writeData(0, sizeof(indices), indices);

    // QCoreApplication::Instance().runMainLoop();

    FSamplerStateDesc samplerStateDesc{};
    samplerStateDesc.minFilter = EFilterOptions::Point;
    samplerStateDesc.magFilter = EFilterOptions::Point;
    samplerStateDesc.mipFilter = EFilterOptions::Point;
    gSamplerState = FSamplerState::New(samplerStateDesc);

    auto texture = StaticResourceCast<FTexture>(gResources().createResourceHandle(FTexture::White, true));

    QCoreApplication::Instance().setIsMainLoopRunning(true);
    while (QCoreApplication::Instance().isMainLoopRunning()) {
        QCoreApplication::Instance().calculateFrameStats();

        gTime().update();

        gRenderWindowManager().update();

        gPerObjectBuffer->write(0, &gPerObject, sizeof(gPerObject));
        gMaterialParamsBuffer->write(0, &gMaterialParams, sizeof(gMaterialParams));

        gRenderAPI().setRenderTarget(gCoreApplication().getPrimaryWindow());
        gRenderAPI().clearRenderTarget(EFrameBufferType::Color | EFrameBufferType::Depth | EFrameBufferType::Stencil);

        // rendering

        gRenderAPI().setGraphicsPipeline(gPipeline);

        auto data = gMesh->getVertexData();

        gRenderAPI().setVertexDeclaration(gMesh->getVertexData()->vertexDeclaration);
        gRenderAPI().setVertexBuffer(0, {gMesh->getVertexData()->getBuffer(0)});
        gRenderAPI().setIndexBuffer(gMesh->getIndexBuffer());

        gGpuParams->setParamBlockBuffer(TEXT("PerObject"), gPerObjectBuffer);
        gGpuParams->setParamBlockBuffer(TEXT("MaterialParams"), gMaterialParamsBuffer);
        gGpuParams->setTexture(EGpuProgramType::Fragment, TEXT("gTexture"), texture);
        gGpuParams->setSamplerState(EGpuProgramType::Fragment, TEXT("gSamplerState"), gSamplerState);
        gRenderAPI().setGpuParams(gGpuParams);

        gRenderAPI().drawIndexed(0, 3, 0, 3);
        gRenderAPI().swapBuffer(gCoreApplication().getPrimaryWindow());
    }

    texture->destroy();
    delete gSamplerState;

    delete gVertexDeclaration;
    delete gGpuParams;
    delete gPipeline;

    QCoreApplication::ShutDown();
}