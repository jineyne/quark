#include "VertexDeclaration.h"
#include "Manager/BufferManager.h"
#include "Exception/Exception.h"

FVertexElement::FVertexElement(uint16_t source, uint32_t offset, EVertexElementType type,
                               EVertexElementSemantic semantic, uint32_t index, uint32_t instanceStepRate)
        : mSource(source), mOffset(offset), mType(type), mSemantic(semantic), mIndex(index),
        mInstanceStepRate(instanceStepRate) {}

uint32_t FVertexElement::GetTypeSize(EVertexElementType type) {
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

uint32_t FVertexElement::GetTypeCount(EVertexElementType type) {
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

size_t FVertexElement::GetHash(const FVertexElement &element) {
    size_t hash = 0;

    CombineHash(hash, element.mType);
    CombineHash(hash, element.mSource);
    CombineHash(hash, element.mIndex);
    CombineHash(hash, element.mOffset);
    CombineHash(hash, element.mSemantic);
    CombineHash(hash, element.mInstanceStepRate);

    return hash;
}

bool FVertexElement::operator==(const FVertexElement &rhs) const {
    return ((mType == rhs.mType) && (mSemantic == rhs.mSemantic) && (mSource == rhs.mSource) &&
            (mOffset == rhs.mOffset) && (mIndex == rhs.mIndex) && (mInstanceStepRate == rhs.mInstanceStepRate));
}

bool FVertexElement::operator!=(const FVertexElement &rhs) const {
    return !(*this == rhs);
}

uint32_t FVertexElement::getSize() const { return GetTypeSize(mType); }
EVertexElementType FVertexElement::getType() const { return mType; }
EVertexElementSemantic FVertexElement::getSemantic() const { return mSemantic; }
uint32_t FVertexElement::getSource() const { return mSource; }
uint32_t FVertexElement::getOffset() const { return mOffset; }
uint32_t FVertexElement::getSemanticIndex() const { return mIndex; }
uint32_t FVertexElement::getInstanceStepRate() const { return mInstanceStepRate; }

FVertexDeclaration::FVertexDeclaration(const TArray<FVertexElement> &elements) {
    for (auto &elem : elements) {
        EVertexElementType type = elem.getType();

        mElementList.add(FVertexElement(elem.getSource(), elem.getOffset(), type, elem.getSemantic(), elem.getSemanticIndex(), elem.getInstanceStepRate()));
    }
}

FVertexDeclaration *FVertexDeclaration::New(FVertexDataDesc *desc) {
    return FBufferManager::Instance().createVertexDeclaration(desc);
}

bool FVertexDeclaration::isCompatible(const FVertexDeclaration *shaderDeclare) {
    const TArray<FVertexElement> &shaderElems = shaderDeclare->getElements();
    const TArray<FVertexElement> &bufferElems = getElements();

    for (auto shaderIter = shaderElems.begin(); shaderIter != shaderElems.end(); ++shaderIter) {
        const FVertexElement *foundElement = nullptr;
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

TArray<FVertexElement> FVertexDeclaration::getMissingElements(FVertexDeclaration *shaderDecl) {
    TArray<FVertexElement> missingElements;

    const TArray<FVertexElement> &shaderElems = shaderDecl->getElements();
    const TArray<FVertexElement> &bufferElems = getElements();

    for (auto shaderIter = shaderElems.begin(); shaderIter != shaderElems.end(); ++shaderIter) {
        const FVertexElement *foundElement = nullptr;
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

const FVertexElement *FVertexDeclaration::getElement(uint32_t index) const {
    assert(index < mElementList.length());

    auto it = mElementList.begin();
    for (uint32_t i = 0; i < index; i++) {
        it++;
    }

    return reinterpret_cast<const FVertexElement *>(&(*it));
}

const FVertexElement *FVertexDeclaration::findElementBySemantic(EVertexElementSemantic semantic) const {
    for (auto &element : mElementList) {
        if (element.getSemantic() == semantic) {
            return reinterpret_cast<const FVertexElement *>(&element);
        }
    }

    return nullptr;
}

uint32_t FVertexDeclaration::getVertexSize() const {
    uint32_t size = 0;
    for (auto &element : mElementList) {
        size += element.getSize();
    }

    return size;
}
