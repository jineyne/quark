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
    Matrix4 gWorldMat;
    Matrix4 gViewMat;
    Matrix4 gProjMat;
} gPerObject;

float vertices[] = {
    -0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.0f,   0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
};

uint32_t indices[] = {
        0, 1, 2,
};

GraphicsPipelineState *gPipeline;
VertexDeclaration *gVertexDeclaration;
VertexBuffer *gVBO;
IndexBuffer *gIBO;
GpuParams *gGpuParams;
GpuParamBlockBuffer *gPerObjectBuffer;

String read(String path) {
    auto file = FileSystem::OpenFile(path);
    auto size = file->size();
    char *buf = (char *) malloc(size + 1);
    file->read(buf, size);
    buf[size] = '\0';

    String result = buf;
    delete buf;

    return result;
}

void loadShader(String path) {
    PipelineStateDesc pipelineStateDesc{};

    GpuProgramDesc vd{};
    vd.type = EGpuProgramType::Vertex;
    vd.source = read(path + "/Color.vs");
    vd.entryPoint = TEXT("ColorVertexShader");
    pipelineStateDesc.vertexProgram = GpuProgram::New(vd);

    GpuProgramDesc fd{};
    fd.type = EGpuProgramType::Fragment;
    fd.source = read(path + "/Color.fs");
    fd.entryPoint = TEXT("ColorPixelShader");
    pipelineStateDesc.fragmentProgram = GpuProgram::New(fd);

    gPipeline = GraphicsPipelineState::New(pipelineStateDesc);

    gPerObjectBuffer = GpuParamBlockBuffer::New(sizeof(gPerObject));
    gPerObject.gProjMat = Matrix4::Perspective(Radian(90), 800.0f / 600.0f, 0.1f, 1000.f);
    gPerObject.gViewMat = Matrix4::Translate(Vector3(0.0f, 0.0f, 5.0f));
    gPerObject.gWorldMat = Matrix4::Identity();
    gPerObjectBuffer->write(0, &gPerObject, sizeof(gPerObject));

    gGpuParams = GpuParams::New(gPipeline);
    gGpuParams->setParamBlockBuffer(TEXT("gPerObject"), gPerObjectBuffer);
}

TEST(QCoreApplicationTest, sycle) {
    ApplicationStartUpDesc desc{};
    desc.renderAPI = TEXT("quark-dx11");

    EXPECT_NO_THROW(CoreApplication::StartUp(desc));

    loadShader("D:/Projects/Quark/Data/Test/Shader");

    auto vdd = VertexDataDesc::New();
    vdd->addElement(EVertexElementType::Float3, EVertexElementSemantic::Position);
    vdd->addElement(EVertexElementType::Color, EVertexElementSemantic::Color);

    gVertexDeclaration = VertexDeclaration::New(vdd);

    VertexBufferDesc vd {};
    vd.usage = EBufferUsage::Static;
    vd.vertexSize = gVertexDeclaration->getVertexSize() * sizeof(float);
    vd.vertexCount = 3;
    gVBO = VertexBuffer::New(vd);
    gVBO->writeData(0, sizeof(vertices), vertices);

    FIndexBufferDesc id {};
    id.usage = EBufferUsage::Static;
    id.indexType = EIndexType::_32bit;
    id.indexCount = 3;
    gIBO = IndexBuffer::New(id);
    gIBO->writeData(0, sizeof(indices), indices);

    // CoreApplication::Instance().runMainLoop();

    CoreApplication::Instance().setIsMainLoopRunning(true);
    while (CoreApplication::Instance().isMainLoopRunning()) {
        CoreApplication::Instance().calculateFrameStats();

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

    EXPECT_NO_THROW(CoreApplication::ShutDown());
}*/
