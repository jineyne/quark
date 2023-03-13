/*
#include "CoreApplication.h"
#include "TestUtility.h"

#include "RenderAPI/VertexBuffer.h"
#include "RenderAPI/GraphicsPipelineState.h"
#include "RenderAPI/GpuParams.h"
#include "RenderAPI/GpuParamBlockBuffer.h"
#include "RenderAPI/GpuProgram.h"
#include "RenderAPI/RenderAPI.h"
#include "RenderAPI/IndexBuffer.h"
#include "RenderAPI/VertexDataDesc.h"
#include "FileSystem/FileSystem.h"
#include "Math/Radian.h"
#include "Math/Vector3.h"
#include "Misc/Time.h"
#include "Manager/RenderWindowManager.h"

struct FPerObject {
    FMatrix4 gWorldMat;
    FMatrix4 gViewMat;
    FMatrix4 gProjMat;
} gPerObject;

float vertices[] = {
    -0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.0f,   0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
};

uint32_t indices[] = {
        0, 1, 2,
};

FGraphicsPipelineState *gPipeline;
FVertexDeclaration *gVertexDeclaration;
FVertexBuffer *gVBO;
FIndexBuffer *gIBO;
FGpuParams *gGpuParams;
FGpuParamBlockBuffer *gPerObjectBuffer;

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
    vd.source = read(path + "/Color.vs");
    vd.entryPoint = TEXT("ColorVertexShader");
    pipelineStateDesc.vertexProgram = FGpuProgram::New(vd);

    FGpuProgramDesc fd{};
    fd.type = EGpuProgramType::Fragment;
    fd.source = read(path + "/Color.fs");
    fd.entryPoint = TEXT("ColorPixelShader");
    pipelineStateDesc.fragmentProgram = FGpuProgram::New(fd);

    gPipeline = FGraphicsPipelineState::New(pipelineStateDesc);

    gPerObjectBuffer = FGpuParamBlockBuffer::New(sizeof(gPerObject));
    gPerObject.gProjMat = FMatrix4::Perspective(FRadian(90), 800.0f / 600.0f, 0.1f, 1000.f);
    gPerObject.gViewMat = FMatrix4::Translate(FVector3(0.0f, 0.0f, 5.0f));
    gPerObject.gWorldMat = FMatrix4::Identity();
    gPerObjectBuffer->write(0, &gPerObject, sizeof(gPerObject));

    gGpuParams = FGpuParams::New(gPipeline);
    gGpuParams->setParamBlockBuffer(TEXT("gPerObject"), gPerObjectBuffer);
}

TEST(QCoreApplicationTest, sycle) {
    FApplicationStartUpDesc desc{};
    desc.renderAPI = TEXT("quark-dx11");

    EXPECT_NO_THROW(QCoreApplication::StartUp(desc));

    loadShader("D:/Projects/Quark/Data/Test/Shader");

    auto vdd = FVertexDataDesc::New();
    vdd->addElement(EVertexElementType::Float3, EVertexElementSemantic::Position);
    vdd->addElement(EVertexElementType::Color, EVertexElementSemantic::Color);

    gVertexDeclaration = FVertexDeclaration::New(vdd);

    FVertexBufferDesc vd {};
    vd.usage = EBufferUsage::Static;
    vd.vertexSize = gVertexDeclaration->getVertexSize() * sizeof(float);
    vd.vertexCount = 3;
    gVBO = FVertexBuffer::New(vd);
    gVBO->writeData(0, sizeof(vertices), vertices);

    FIndexBufferDesc id {};
    id.usage = EBufferUsage::Static;
    id.indexType = EIndexType::_32bit;
    id.indexCount = 3;
    gIBO = FIndexBuffer::New(id);
    gIBO->writeData(0, sizeof(indices), indices);

    // QCoreApplication::Instance().runMainLoop();

    QCoreApplication::Instance().setIsMainLoopRunning(true);
    while (QCoreApplication::Instance().isMainLoopRunning()) {
        QCoreApplication::Instance().calculateFrameStats();

        gTime().update();

        gRenderWindowManager().update();

        gRenderAPI().setRenderTarget(gCoreApplication().getPrimaryWindow());
        gRenderAPI().clearRenderTarget(EFrameBufferType::Color);

        // rendering

        gRenderAPI().setGraphicsPipeline(gPipeline);
        gRenderAPI().setVertexDeclaration(gVertexDeclaration);
        gRenderAPI().setVertexBuffer(0, {gVBO});
        gRenderAPI().setIndexBuffer(gIBO);
        gRenderAPI().setGpuParams(gGpuParams);
        gRenderAPI().drawIndexed(0, 3, 0, 3);

        gRenderAPI().swapBuffer(gCoreApplication().getPrimaryWindow());
    }

    delete gVertexDeclaration;
    delete gGpuParams;
    delete gVBO;
    delete gIBO;
    delete gPipeline;

    EXPECT_NO_THROW(QCoreApplication::ShutDown());
}*/
