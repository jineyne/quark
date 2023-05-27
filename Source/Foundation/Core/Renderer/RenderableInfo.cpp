#include "RenderableInfo.h"

PerObjectParamDef gPerObjectParamDef;
PerCallParamDef gPerCallParamDef;

void PerObjectBuffer::Update(GpuParamBlockBuffer *buffer, const Matrix4 &tfrm, const Matrix4 &tfrmNoScale,
                             const Matrix4 &prevTfrm, uint32_t layer) {
    gPerObjectParamDef.gMatWorld.set(buffer, tfrm);
    /*gPerObjectParamDef.gMatWorldNoScale.set(buffer, tfrmNoScale);
    gPerObjectParamDef.gMatPrevWorld.set(buffer, prevTfrm);
    gPerObjectParamDef.gLayer.set(buffer, (int32_t) layer);*/
}

void RenderableElement::draw() const {
    auto &rapi = RenderAPI::Instance();

    auto *vertexData = mesh->getVertexData();

    rapi.setVertexDeclaration(vertexData->vertexDeclaration);
    const auto &vertexBuffers = vertexData->getBuffers();
    if (vertexBuffers.length() > 0) {
        uint32_t endSlot = 0;
        uint32_t startSlot = 32;

        for (auto pair : vertexBuffers) {
            if (pair.key >= 32) {
                EXCEPT(LogRenderer, InvalidParametersException, TEXT("Buffer is out of bound."));
            }

            startSlot = std::min(pair.key, startSlot);
            endSlot = std::min(pair.key, endSlot);
        }

        TArray<VertexBuffer*> buffers;
        buffers.resize(endSlot - startSlot + 1);

        for (auto pair : vertexBuffers) {
            buffers[pair.key - startSlot] = pair.value;
        }

        rapi.setVertexBuffer(startSlot, buffers);
    }

    auto *indexBuffer = mesh->getIndexBuffer();
    rapi.setIndexBuffer(indexBuffer);

    auto indexCount = indexBuffer->getIndexCount();
    rapi.drawIndexed(0, indexCount, 0, vertexData->vertexCount);
}

RenderableInfo::RenderableInfo() {
    perObjectParamBuffer = gPerObjectParamDef.createBuffer();
    perCallParamBuffer = gPerCallParamDef.createBuffer();
}

RenderableInfo::~RenderableInfo() {
    q_delete(perObjectParamBuffer);
    q_delete(perCallParamBuffer);
}

void RenderableInfo::updatePerObjectBuffer() {
    const Matrix4 worldNoScaleTransform = renderable->getMatrix();
    PerObjectBuffer::Update(perObjectParamBuffer, worldMatrix, worldNoScaleTransform, prevWorldMatrix, renderable->getLayer());
}

void RenderableInfo::updatePerCallBuffer(const Matrix4 &view, bool flush) {
    const Matrix4 worldViewProjMatrix = renderable->getMatrix() * view;

    gPerCallParamDef.gMatWorldViewProj.set(perCallParamBuffer, worldViewProjMatrix);

    if (flush) {
        perCallParamBuffer->flushToGPU();
    }
}
