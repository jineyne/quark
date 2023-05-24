#include "RenderableInfo.h"

PerObjectParamDef gPerObjectParamDef;
PerCallParamDef gPerCallParamDef;

void FPerObjectBuffer::Update(FGpuParamBlockBuffer *buffer, const FMatrix4 &tfrm, const FMatrix4 &tfrmNoScale,
                              const FMatrix4 &prevTfrm, uint32_t layer) {
    gPerObjectParamDef.gMatWorld.set(buffer, tfrm);
    /*gPerObjectParamDef.gMatWorldNoScale.set(buffer, tfrmNoScale);
    gPerObjectParamDef.gMatPrevWorld.set(buffer, prevTfrm);
    gPerObjectParamDef.gLayer.set(buffer, (int32_t) layer);*/
}

void FRenderableElement::draw() const {
    auto &rapi = FRenderAPI::Instance();

    auto *vertexData = mesh->getVertexData();

    rapi.setVertexDeclaration(vertexData->vertexDeclaration);
    const auto &vertexBuffers = vertexData->getBuffers();
    if (vertexBuffers.length() > 0) {
        uint32_t endSlot = 0;
        uint32_t startSlot = 32;

        for (auto pair : vertexBuffers) {
            if (pair.key >= 32) {
                EXCEPT(FLogRenderer, InvalidParametersException, TEXT("Buffer is out of bound."));
            }

            startSlot = std::min(pair.key, startSlot);
            endSlot = std::min(pair.key, endSlot);
        }

        TArray<FVertexBuffer*> buffers;
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

FRenderableInfo::FRenderableInfo() {
    perObjectParamBuffer = gPerObjectParamDef.createBuffer();
    perCallParamBuffer = gPerCallParamDef.createBuffer();
}

FRenderableInfo::~FRenderableInfo() {
    q_delete(perObjectParamBuffer);
    q_delete(perCallParamBuffer);
}

void FRenderableInfo::updatePerObjectBuffer() {
    const FMatrix4 worldNoScaleTransform = renderable->getMatrix();
    FPerObjectBuffer::Update(perObjectParamBuffer, worldMatrix, worldNoScaleTransform, prevWorldMatrix, renderable->getLayer());
}

void FRenderableInfo::updatePerCallBuffer(const FMatrix4 &view, bool flush) {
    const FMatrix4 worldViewProjMatrix = renderable->getMatrix() * view;

    gPerCallParamDef.gMatWorldViewProj.set(perCallParamBuffer, worldViewProjMatrix);

    if (flush) {
        perCallParamBuffer->flushToGPU();
    }
}
