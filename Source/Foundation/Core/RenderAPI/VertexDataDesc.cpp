#include "VertexDataDesc.h"

VertexDataDesc *VertexDataDesc::New() {
    return q_new<VertexDataDesc>();
}

void VertexDataDesc::addElement(EVertexElementType type, EVertexElementSemantic semantic, uint32_t semanticIdx,
                                uint32_t instanceStepRate) {
    mElementList.add(VertexElement(0, 0, type, semantic, semanticIdx, instanceStepRate));
}

bool VertexDataDesc::hasElement(EVertexElementSemantic semantic, uint32_t semanticIdx) const {
    auto it = mElementList.findIf([semantic, semanticIdx](const VertexElement &x) {
        return x.getSemantic() == semantic && x.getSemanticIndex() == semanticIdx;
    });

    if (it != nullptr) {
        return true;
    }

    return false;
}

uint32_t VertexDataDesc::getElementSize(EVertexElementSemantic semantic) const {
    for (auto &element : mElementList) {
        if (element.getSemantic() == semantic) {
            return element.getSize();
        }
    }

    return -1;
}

uint32_t VertexDataDesc::getElementOffsetFromStream(EVertexElementSemantic semantic, uint32_t semanticIdx) const {
    uint32_t offset = 0;

    for (auto &element : mElementList) {
        if (element.getSemantic() == semantic && element.getSemanticIndex() == semanticIdx) {
            break;
        }

        offset += element.getSize();
    }

    return offset;
}

uint32_t VertexDataDesc::getVertexStride() const {
    uint32_t vertexStride = 0;
    for (auto &element : mElementList) {
        vertexStride += element.getSize();
    }

    return vertexStride;
}

const VertexElement *VertexDataDesc::getElement(EVertexElementSemantic semantic) const {
    for (auto &element : mElementList) {
        if (element.getSemantic() == semantic) {
            return &element;
        }
    }

    return nullptr;
}

const uint32_t VertexDataDesc::getElementCount() const {
    return mElementList.length();
}

TArray<VertexElement> VertexDataDesc::createElements() const {
    TArray<VertexElement> declarationElements;
    uint32_t offset = 0;
    for (auto &element : mElementList) {
        declarationElements.add(VertexElement(element.getSource(), offset, element.getType(), element.getSemantic(),
                                              element.getSemanticIndex(), element.getInstanceStepRate()));
        offset += element.getSize();
    }
    return declarationElements;
}
