#include "VertexDeclaration.h"
#include "Manager/BufferManager.h"
#include "Exception/Exception.h"

VertexElement::VertexElement(uint16_t source, uint32_t offset, EVertexElementType type,
                             EVertexElementSemantic semantic, uint32_t index, uint32_t instanceStepRate)
        : mSource(source), mOffset(offset), mType(type), mSemantic(semantic), mIndex(index),
        mInstanceStepRate(instanceStepRate) {}

uint32_t VertexElement::GetTypeSize(EVertexElementType type) {
    switch (type) {
        case EVertexElementType::Float1:
            return sizeof(float);

        case EVertexElementType::Float2:
            return sizeof(float) * 2;

        case EVertexElementType::Float3:
            return sizeof(float) * 3;

        case EVertexElementType::Float4:
            return sizeof(float) * 4;

        default:
            break;
    }

    return 0;
}

uint32_t VertexElement::GetTypeCount(EVertexElementType type) {
    switch (type) {
        case EVertexElementType::Float1:
            return 1;

        case EVertexElementType::Float2:
            return 2;

        case EVertexElementType::Float3:
            return 3;

        case EVertexElementType::Float4:
            return 4;

        default:
            break;
    }

    EXCEPT(FLogRenderAPI, RenderAPIException, TEXT("The given vertex element type is an invalid type."));
    return 0;
}

size_t VertexElement::GetHash(const VertexElement &element) {
    size_t hash = 0;

    CombineHash(hash, element.mType);
    CombineHash(hash, element.mSource);
    CombineHash(hash, element.mIndex);
    CombineHash(hash, element.mOffset);
    CombineHash(hash, element.mSemantic);
    CombineHash(hash, element.mInstanceStepRate);

    return hash;
}

bool VertexElement::operator==(const VertexElement &rhs) const {
    return ((mType == rhs.mType) && (mSemantic == rhs.mSemantic) && (mSource == rhs.mSource) &&
            (mOffset == rhs.mOffset) && (mIndex == rhs.mIndex) && (mInstanceStepRate == rhs.mInstanceStepRate));
}

bool VertexElement::operator!=(const VertexElement &rhs) const {
    return !(*this == rhs);
}

uint32_t VertexElement::getSize() const { return GetTypeSize(mType); }
EVertexElementType VertexElement::getType() const { return mType; }
EVertexElementSemantic VertexElement::getSemantic() const { return mSemantic; }
uint32_t VertexElement::getSource() const { return mSource; }
uint32_t VertexElement::getOffset() const { return mOffset; }
uint32_t VertexElement::getSemanticIndex() const { return mIndex; }
uint32_t VertexElement::getInstanceStepRate() const { return mInstanceStepRate; }

VertexDeclaration::VertexDeclaration(const TArray<VertexElement> &elements) {
    for (auto &elem : elements) {
        EVertexElementType type = elem.getType();

        mElementList.add(VertexElement(elem.getSource(), elem.getOffset(), type, elem.getSemantic(), elem.getSemanticIndex(), elem.getInstanceStepRate()));
    }
}

VertexDeclaration *VertexDeclaration::New(VertexDataDesc *desc) {
    return BufferManager::Instance().createVertexDeclaration(desc);
}

bool VertexDeclaration::isCompatible(const VertexDeclaration *shaderDeclare) {
    const TArray<VertexElement> &shaderElems = shaderDeclare->getElements();
    const TArray<VertexElement> &bufferElems = getElements();

    for (auto shaderIter = shaderElems.begin(); shaderIter != shaderElems.end(); ++shaderIter) {
        const VertexElement *foundElement = nullptr;
        for (auto bufferIter = bufferElems.begin(); bufferIter != bufferElems.end(); ++bufferIter) {
            if (shaderIter->getSemantic() == bufferIter->getSemantic()) {
                foundElement = &(*bufferIter);
                break;
            }
        }

        if (foundElement == nullptr) {
            return false;
        }
    }

    return true;
}

TArray<VertexElement> VertexDeclaration::getMissingElements(VertexDeclaration *shaderDecl) {
    TArray<VertexElement> missingElements;

    const TArray<VertexElement> &shaderElems = shaderDecl->getElements();
    const TArray<VertexElement> &bufferElems = getElements();

    for (auto shaderIter = shaderElems.begin(); shaderIter != shaderElems.end(); ++shaderIter) {
        const VertexElement *foundElement = nullptr;
        for (auto bufferIter = bufferElems.begin(); bufferIter != bufferElems.end(); ++bufferIter) {
            if (shaderIter->getSemantic() == bufferIter->getSemantic()) {
                foundElement = &(*bufferIter);
                break;
            }
        }

        if (foundElement == nullptr)
            missingElements.add(*shaderIter);
    }

    return missingElements;
}

const VertexElement *VertexDeclaration::getElement(uint32_t index) const {
    assert(index < mElementList.length());

    auto it = mElementList.begin();
    for (uint32_t i = 0; i < index; i++) {
        it++;
    }

    return reinterpret_cast<const VertexElement *>(&(*it));
}

const VertexElement *VertexDeclaration::findElementBySemantic(EVertexElementSemantic semantic) const {
    for (auto &element : mElementList) {
        if (element.getSemantic() == semantic) {
            return reinterpret_cast<const VertexElement *>(&element);
        }
    }

    return nullptr;
}

uint32_t VertexDeclaration::getVertexSize() const {
    uint32_t size = 0;
    for (auto &element : mElementList) {
        size += element.getSize();
    }

    return size;
}
