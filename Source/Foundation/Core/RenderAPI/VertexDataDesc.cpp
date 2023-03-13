#include "VertexDataDesc.h"

FVertexDataDesc *FVertexDataDesc::New() {
    return new FVertexDataDesc();
}

void FVertexDataDesc::addElement(EVertexElementType type, EVertexElementSemantic semantic, uint32_t semanticIdx,
                                 uint32_t instanceStepRate) {
    mElementList.add(FVertexElement(0, 0, type, semantic, semanticIdx, instanceStepRate));
}

bool FVertexDataDesc::hasElement(EVertexElementSemantic semantic, uint32_t semanticIdx) const {
    auto it = mElementList.findIf([semantic, semanticIdx](const FVertexElement &x) {
        return x.getSemantic() == semantic && x.getSemanticIndex() == semanticIdx;
    });

    if (it != nullptr) {
        return true;
    }

    return false;
}

uint32_t FVertexDataDesc::getElementSize(EVertexElementSemantic semantic) const {
    for (auto &element : mElementList) {
        if (element.getSemantic() == semantic) {
            return element.getSize();
        }
    }

    return -1;
}

uint32_t FVertexDataDesc::getElementOffsetFromStream(EVertexElementSemantic semantic, uint32_t semanticIdx) const {
    uint32_t offset = 0;

    for (auto &element : mElementList) {
        if (element.getSemantic() == semantic && element.getSemanticIndex() == semanticIdx) {
            break;
        }

        offset += element.getSize();
    }

    return offset;
}

uint32_t FVertexDataDesc::getVertexStride() const {
    uint32_t vertexStride = 0;
    for (auto &element : mElementList) {
        vertexStride += element.getSize();
    }

    return vertexStride;
}

const FVertexElement *FVertexDataDesc::getElement(EVertexElementSemantic semantic) const {
    for (auto &element : mElementList) {
        if (element.getSemantic() == semantic) {
            return &element;
        }
    }

    return nullptr;
}

const uint32_t FVertexDataDesc::getElementCount() const {
    return mElementList.length();
}

TArray<FVertexElement> FVertexDataDesc::createElements() const {
    TArray<FVertexElement> declarationElements;
    uint32_t offset = 0;
    for (auto &element : mElementList) {
        declarationElements.add(FVertexElement(element.getSource(), offset, element.getType(), element.getSemantic(),
                                               element.getSemanticIndex(), element.getInstanceStepRate()));
        offset += element.getSize();
    }
    return declarationElements;
}
