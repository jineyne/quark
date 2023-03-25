#include "MeshData.h"
#include "Mesh/MeshType.h"
#include "RenderAPI/VertexDataDesc.h"

FMeshData::FMeshData(uint32_t vertexCount, uint32_t indexCount, FVertexDataDesc *vertexDesc, EIndexType indexType)
        : mVertexCount(vertexCount), mIndexCount(indexCount), mVertexDesc(vertexDesc), mIndexType(indexType) {
    allocateInternalBuffer();
}

FMeshData::~FMeshData() {

}

FMeshData *FMeshData::New(uint32_t vertexCount, uint32_t indexCount, FVertexDataDesc *vertexDesc, EIndexType indexType) {
    return q_new<FMeshData>(vertexCount, indexCount, vertexDesc, indexType);
}

FMeshData *FMeshData::Combine(const TArray<FMeshData *> &meshs) {
    if (meshs.empty()) {
        return FMeshData::New(0, 0, FVertexDataDesc::New());
    }

    if (meshs.length() == 1) {
        return meshs[0];
    }
    uint32_t totalVertexCount = 0;
    uint32_t totalIndexCount = 0;

    for (auto &mesh : meshs) {
        totalVertexCount += mesh->getVertexCount();
        totalIndexCount += mesh->getIndexCount();
    }

    auto vertexData = FVertexDataDesc::New();
    TArray<FVertexElement> vertexElements{};

    for (auto &mesh : meshs) {
        uint32_t count = mesh->getVertexDesc()->getElementCount();
        for (uint32_t i = 0; i < count; i++) {
            auto newElement = mesh->getVertexDesc()->getElement(i);
            int32_t foundIdx = -1;
            uint32_t idx = 0;

            for (auto &oldElement : vertexElements) {
                if (oldElement == newElement) {
                    if (oldElement.getType() != newElement.getType()) {
                        EXCEPT(FLogMesh, RenderAPIException, TEXT("Two elements have same semantics but different types. This is not supported."));
                    }

                    foundIdx = idx;
                    break;
                }

                idx++;
            }

            if (foundIdx == -1) {
                vertexElements.add(newElement);
                vertexData->addElement(newElement.getType(), newElement.getSemantic(), newElement.getSemanticIndex());
            }
        }
    }

    auto result = FMeshData::New(totalVertexCount, totalIndexCount, vertexData);

    uint32_t vertexOffset = 0;
    uint32_t indexOffset = 0;
    auto idxPtr = (uint32_t *)result->getIndexData();

    for (auto &mesh : meshs) {
        auto indicesCount = mesh->getIndexCount();
        auto srcData = (uint32_t *)mesh->getIndexData();

        for (uint32_t i = 0; i < indicesCount; i++) {
            idxPtr[i] = srcData[i] + vertexOffset;
        }

        indexOffset += indicesCount;
        idxPtr += indicesCount;
        vertexOffset += mesh->getVertexCount();
    }

    vertexOffset = 0;
    for (auto &mesh : meshs) {
        for (auto &element : vertexElements) {
            auto dstVertexStride = vertexData->getVertexStride();
            auto dstData = const_cast<uint8_t *>(result->getElementData(element.getSemantic(),
                                                                        element.getSemanticIndex()));
            dstData += vertexOffset * dstVertexStride;

            auto srcVerticesCount = mesh->getVertexCount();
            auto vertexSize = vertexData->getElementSize(element.getSemantic());

            if (mesh->getVertexDesc()->hasElement(element.getSemantic(), element.getSemanticIndex())) {
                auto srcVertexStride = mesh->getVertexDesc()->getVertexStride();
                auto srcData = mesh->getElementData(element.getSemantic(), element.getSemanticIndex());

                for (uint32_t i = 0; i < srcVerticesCount; i++) {
                    memcpy(dstData, srcData, vertexSize);
                    dstData += dstVertexStride;
                    srcData += srcVertexStride;
                }
            } else {
                for (uint32_t i = 0; i < srcVerticesCount; i++) {
                    memset(dstData, 0, vertexSize);
                    dstData += dstVertexStride;
                }
            }
        }

        vertexOffset += mesh->getVertexCount();
    }

    return result;
}

void FMeshData::setVertexData(EVertexElementSemantic semantic, void *data, uint32_t size, uint32_t semanticIdx) {
    assert(data != nullptr);
    if (!mVertexDesc->hasElement(semantic, semanticIdx)) {
        LOG(FLogMesh, Warning, TEXT("MeshData doesn't contain an element of specified type: Semantic: %d, Semantic index: %d"), semantic, semanticIdx);
    }

    auto elementSize = mVertexDesc->getElementSize(semantic);
    auto totalSize = elementSize * mVertexCount;

    if (totalSize != size) {
        EXCEPT(FLogMesh, RenderAPIException, TEXT("Buffer sizes don't match. Expected: %d. Got: %d"), totalSize, size);
    }

    auto indexBufferOffset = getIndexBufferSize();
    auto elementOffset = getElementOffset(semantic, semanticIdx);
    auto vertexStride = mVertexDesc->getVertexStride();

    auto dst = getData() + indexBufferOffset + elementOffset;
    auto src = (uint8_t *)data;
    for (uint32_t i = 0; i < mVertexCount; i++) {
        memcpy(dst, src, elementSize);
        dst += vertexStride;
        src += elementSize;
    }
}

void FMeshData::getVertexData(EVertexElementSemantic semantic, void *data, uint32_t size, uint32_t semanticIdx) {
    assert(data != nullptr);
    if (!mVertexDesc->hasElement(semantic, semanticIdx)) {
        LOG(FLogMesh, Warning, TEXT("MeshData doesn't contain an element of specified type: Semantic: %d. Semantic index: %d"), semantic, semanticIdx);
    }

    auto elementSize = mVertexDesc->getElementSize(semantic);
    auto totalSize = elementSize * mVertexCount;

    if (totalSize != size) {
        EXCEPT(FLogMesh, RenderAPIException, TEXT("Buffer sizes don't match. Expected: %d. Got: %d"), totalSize, size);
    }

    auto indexBufferOffset = getIndexBufferSize();
    auto elementOffset = getElementOffset(semantic, semanticIdx);
    auto vertexStride = mVertexDesc->getVertexStride();

    auto src = getData() + indexBufferOffset * elementOffset;
    auto dst = (uint8_t *)data;

    for (uint32_t i = 0; i < mVertexCount; i++) {
        memcpy(dst, src, elementSize);
        dst += elementSize;
        src += vertexStride;
    }
}

VertexElemIter<FVector2> FMeshData::getVec2DataIter(EVertexElementSemantic semantic, uint32_t semanticIdx, uint32_t streamIdx) {
    uint8_t* data;
    uint32_t vertexStride;
    getDataForIterator(semantic, semanticIdx, streamIdx, data, vertexStride);

    return VertexElemIter<FVector2>(data, vertexStride, mVertexCount);
}

VertexElemIter<FVector3> FMeshData::getVec3DataIter(EVertexElementSemantic semantic, uint32_t semanticIdx, uint32_t streamIdx) {
    uint8_t* data;
    uint32_t vertexStride;
    getDataForIterator(semantic, semanticIdx, streamIdx, data, vertexStride);

    return VertexElemIter<FVector3>(data, vertexStride, mVertexCount);
}

VertexElemIter<uint32_t> FMeshData::getDWORDDataIter(EVertexElementSemantic semantic, uint32_t semanticIdx, uint32_t streamIdx) {
    uint8_t* data;
    uint32_t vertexStride;
    getDataForIterator(semantic, semanticIdx, streamIdx, data, vertexStride);

    return VertexElemIter<uint32_t>(data, vertexStride, mVertexCount);
}

void FMeshData::getDataForIterator(EVertexElementSemantic semantic, uint32_t semanticIdx, uint32_t streamIdx, uint8_t *&data,
                              uint32_t &stride) const {
    if(!mVertexDesc->hasElement(semantic, semanticIdx)) {
        EXCEPT(FLogMesh, InvalidParametersException, TEXT("MeshData doesn't contain an element of specified type: Semantic: %d, Semantic index: %d, Stream index: %d"), semantic, semanticIdx, streamIdx);
    }

    uint32_t indexBufferOffset = getIndexBufferSize();

    uint32_t elementOffset = getElementOffset(semantic, semanticIdx);

    data = getData() + indexBufferOffset + elementOffset;
    stride = mVertexDesc->getVertexStride();
}

uint8_t *FMeshData::getElementData(EVertexElementSemantic semantic, uint32_t semanticIdx) const {
    return getData() + getIndexBufferSize() + getElementOffset(semantic, semanticIdx);
}

uint8_t *FMeshData::getStreamData() const {
    return getData() + getIndexBufferSize();
}

uint32_t FMeshData::getIndexElementSize() const {
    switch (mIndexType) {
        case EIndexType::_16bit:
            return sizeof(uint16_t);

        default:
        case EIndexType::_32bit:
            return sizeof(uint32_t);
    }
}

uint32_t FMeshData::getElementOffset(EVertexElementSemantic semantic, uint32_t semanticIdx) const {
    return mVertexDesc->getElementOffsetFromStream(semantic, semanticIdx);
}

uint8_t *FMeshData::getIndexData() const {
    return getData();
}

uint16_t *FMeshData::getIndex16() const {
    if (mIndexType != EIndexType::_16bit) {
        EXCEPT(FLogMesh, InternalErrorException, TEXT("Attempting to get 16bit index buffer, but internally allocated buffer is 32 bit."));
    }

    return reinterpret_cast<uint16_t *>(getData());
}

uint32_t *FMeshData::getIndex32() const {
    if (mIndexType != EIndexType::_32bit) {
        EXCEPT(FLogMesh, InternalErrorException, TEXT("Attempting to get 32bit index buffer, but internally allocated buffer is 16 bit."));
    }

    return reinterpret_cast<uint32_t *>(getData());
}

uint32_t FMeshData::getVertexCount() const {
    return mVertexCount;
}

uint32_t FMeshData::getIndexCount() const {
    return mIndexCount;
}

uint32_t FMeshData::getStreamSize() const {
    return mVertexDesc->getVertexStride() * mVertexCount;
}

uint32_t FMeshData::getIndexBufferOffset() const {
    return 0;
}

uint32_t FMeshData::getIndexBufferSize() const {
    switch (mIndexType) {
        case EIndexType::_16bit:
            return mIndexCount * sizeof(uint16_t);

        default:
        case EIndexType::_32bit:
            return mIndexCount * sizeof(uint32_t);
    }
}

FVertexDataDesc *FMeshData::getVertexDesc() const {
    return mVertexDesc;
}

uint32_t FMeshData::getInternalBufferSize() const {
    return getIndexBufferSize() + getStreamSize();
}
