#include "MeshData.h"
#include "Mesh/MeshType.h"
#include "RenderAPI/VertexDataDesc.h"

MeshData::MeshData(uint32_t vertexCount, uint32_t indexCount, VertexDataDesc *vertexDesc, EIndexType indexType)
        : mVertexCount(vertexCount), mIndexCount(indexCount), mVertexDesc(vertexDesc), mIndexType(indexType) {
    allocateInternalBuffer();
}

MeshData::~MeshData() {

}

MeshData *MeshData::New(uint32_t vertexCount, uint32_t indexCount, VertexDataDesc *vertexDesc, EIndexType indexType) {
    return q_new<MeshData>(vertexCount, indexCount, vertexDesc, indexType);
}

MeshData *MeshData::Combine(const TArray<MeshData *> &meshs) {
    if (meshs.empty()) {
        return MeshData::New(0, 0, VertexDataDesc::New());
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

    auto vertexData = VertexDataDesc::New();
    TArray<VertexElement> vertexElements{};

    for (auto &mesh : meshs) {
        uint32_t count = mesh->getVertexDesc()->getElementCount();
        for (uint32_t i = 0; i < count; i++) {
            auto newElement = mesh->getVertexDesc()->getElement(i);
            int32_t foundIdx = -1;
            uint32_t idx = 0;

            for (auto &oldElement : vertexElements) {
                if (oldElement == newElement) {
                    if (oldElement.getType() != newElement.getType()) {
                        EXCEPT(LogMesh, RenderAPIException, TEXT("Two elements have same semantics but different types. This is not supported."));
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

    auto result = MeshData::New(totalVertexCount, totalIndexCount, vertexData);

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

void MeshData::setVertexData(EVertexElementSemantic semantic, void *data, uint32_t size, uint32_t semanticIdx) {
    assert(data != nullptr);
    if (!mVertexDesc->hasElement(semantic, semanticIdx)) {
        LOG(LogMesh, Warning, TEXT("MeshData doesn't contain an element of specified type: Semantic: %d, Semantic index: %d"), semantic, semanticIdx);
    }

    auto elementSize = mVertexDesc->getElementSize(semantic);
    auto totalSize = elementSize * mVertexCount;

    if (totalSize != size) {
        EXCEPT(LogMesh, RenderAPIException, TEXT("Buffer sizes don't match. Expected: %d. Got: %d"), totalSize, size);
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

void MeshData::getVertexData(EVertexElementSemantic semantic, void *data, uint32_t size, uint32_t semanticIdx) {
    assert(data != nullptr);
    if (!mVertexDesc->hasElement(semantic, semanticIdx)) {
        LOG(LogMesh, Warning, TEXT("MeshData doesn't contain an element of specified type: Semantic: %d. Semantic index: %d"), semantic, semanticIdx);
    }

    auto elementSize = mVertexDesc->getElementSize(semantic);
    auto totalSize = elementSize * mVertexCount;

    if (totalSize != size) {
        EXCEPT(LogMesh, RenderAPIException, TEXT("Buffer sizes don't match. Expected: %d. Got: %d"), totalSize, size);
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

VertexElemIter<Vector2> MeshData::getVec2DataIter(EVertexElementSemantic semantic, uint32_t semanticIdx, uint32_t streamIdx) {
    uint8_t* data;
    uint32_t vertexStride;
    getDataForIterator(semantic, semanticIdx, streamIdx, data, vertexStride);

    return VertexElemIter<Vector2>(data, vertexStride, mVertexCount);
}

VertexElemIter<Vector3> MeshData::getVec3DataIter(EVertexElementSemantic semantic, uint32_t semanticIdx, uint32_t streamIdx) {
    uint8_t* data;
    uint32_t vertexStride;
    getDataForIterator(semantic, semanticIdx, streamIdx, data, vertexStride);

    return VertexElemIter<Vector3>(data, vertexStride, mVertexCount);
}

VertexElemIter<uint32_t> MeshData::getDWORDDataIter(EVertexElementSemantic semantic, uint32_t semanticIdx, uint32_t streamIdx) {
    uint8_t* data;
    uint32_t vertexStride;
    getDataForIterator(semantic, semanticIdx, streamIdx, data, vertexStride);

    return VertexElemIter<uint32_t>(data, vertexStride, mVertexCount);
}

void MeshData::getDataForIterator(EVertexElementSemantic semantic, uint32_t semanticIdx, uint32_t streamIdx, uint8_t *&data,
                                  uint32_t &stride) const {
    if(!mVertexDesc->hasElement(semantic, semanticIdx)) {
        EXCEPT(LogMesh, InvalidParametersException, TEXT("MeshData doesn't contain an element of specified type: Semantic: %d, Semantic index: %d, Stream index: %d"), semantic, semanticIdx, streamIdx);
    }

    uint32_t indexBufferOffset = getIndexBufferSize();

    uint32_t elementOffset = getElementOffset(semantic, semanticIdx);

    data = getData() + indexBufferOffset + elementOffset;
    stride = mVertexDesc->getVertexStride();
}

uint8_t *MeshData::getElementData(EVertexElementSemantic semantic, uint32_t semanticIdx) const {
    return getData() + getIndexBufferSize() + getElementOffset(semantic, semanticIdx);
}

uint8_t *MeshData::getStreamData() const {
    return getData() + getIndexBufferSize();
}

uint32_t MeshData::getIndexElementSize() const {
    switch (mIndexType) {
        case EIndexType::_16bit:
            return sizeof(uint16_t);

        default:
        case EIndexType::_32bit:
            return sizeof(uint32_t);
    }
}

uint32_t MeshData::getElementOffset(EVertexElementSemantic semantic, uint32_t semanticIdx) const {
    return mVertexDesc->getElementOffsetFromStream(semantic, semanticIdx);
}

uint8_t *MeshData::getIndexData() const {
    return getData();
}

uint16_t *MeshData::getIndex16() const {
    if (mIndexType != EIndexType::_16bit) {
        EXCEPT(LogMesh, InternalErrorException, TEXT("Attempting to get 16bit index buffer, but internally allocated buffer is 32 bit."));
    }

    return reinterpret_cast<uint16_t *>(getData());
}

uint32_t *MeshData::getIndex32() const {
    if (mIndexType != EIndexType::_32bit) {
        EXCEPT(LogMesh, InternalErrorException, TEXT("Attempting to get 32bit index buffer, but internally allocated buffer is 16 bit."));
    }

    return reinterpret_cast<uint32_t *>(getData());
}

uint32_t MeshData::getVertexCount() const {
    return mVertexCount;
}

uint32_t MeshData::getIndexCount() const {
    return mIndexCount;
}

uint32_t MeshData::getStreamSize() const {
    return mVertexDesc->getVertexStride() * mVertexCount;
}

uint32_t MeshData::getIndexBufferOffset() const {
    return 0;
}

uint32_t MeshData::getIndexBufferSize() const {
    switch (mIndexType) {
        case EIndexType::_16bit:
            return mIndexCount * sizeof(uint16_t);

        default:
        case EIndexType::_32bit:
            return mIndexCount * sizeof(uint32_t);
    }
}

VertexDataDesc *MeshData::getVertexDesc() const {
    return mVertexDesc;
}

uint32_t MeshData::getInternalBufferSize() const {
    return getIndexBufferSize() + getStreamSize();
}
