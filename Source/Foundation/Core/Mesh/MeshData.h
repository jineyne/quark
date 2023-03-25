#pragma once

#include "CorePrerequisites.h"
#include "Resource/GpuResourceData.h"
#include "Exception/Exception.h"
#include "RenderAPI/IndexBuffer.h"
#include "RenderAPI/VertexDeclaration.h"

template<typename T>
class VertexElemIter {
private:
    uint8_t *mData = nullptr;

    uint8_t *mEnd = nullptr;

    uint32_t mStrideByte = 0;

    uint32_t mElementCount = 0;

public:
    VertexElemIter() {}

    VertexElemIter(uint8_t *data, uint32_t strideByte, uint32_t elementCount)
            : mData(data), mStrideByte(strideByte), mElementCount(elementCount) {
        mEnd = mData + mStrideByte * mElementCount;
    }

public:
    bool addValue(const T &value) {
        setValue(value);
        return moveNext();
    }

    void setValue(const T &value) {
        memcpy(mData, &value, sizeof(T));
    }

    T &getValue() {
        return *((T *)mData);
    }

    bool moveNext() {
#ifdef DEBUG_MODE
        if (mData >= mEnd) {
            EXCEPT(FLogResource, RenderAPIException, TEXT("Vertex element index is out of bound."));
        }
#endif

        mData += mStrideByte;
        return mData <= mEnd;
    }

    uint32_t getElementCount() const {
        return mElementCount;
    }
};

class DLL_EXPORT FMeshData : public FGpuResourceData {
protected:
    uint32_t mDescBuilding = 0;
    uint32_t mVertexCount = 0;
    uint32_t mIndexCount = 0;

    EIndexType mIndexType;
    FVertexDataDesc *mVertexDesc;

public:
    FMeshData(uint32_t vertexCount, uint32_t indexCount, FVertexDataDesc *vertexDesc, EIndexType indexType = EIndexType::_32bit);
    ~FMeshData();

public:
    static FMeshData *New(uint32_t vertexCount, uint32_t indexCount, FVertexDataDesc *vertexDesc, EIndexType indexType = EIndexType::_32bit);
    static FMeshData *Combine(const TArray<FMeshData *> &meshs);

public:
    void setVertexData(EVertexElementSemantic semantic, void *data, uint32_t size, uint32_t semanticIdx = 0);
    void getVertexData(EVertexElementSemantic semantic, void *data, uint32_t size, uint32_t semanticIdx = 0);

    VertexElemIter<FVector2> getVec2DataIter(EVertexElementSemantic semantic, uint32_t semanticIdx = 0, uint32_t streamIdx = 0);
    VertexElemIter<FVector3> getVec3DataIter(EVertexElementSemantic semantic, uint32_t semanticIdx = 0, uint32_t streamIdx = 0);
    VertexElemIter<uint32_t> getDWORDDataIter(EVertexElementSemantic semantic, uint32_t semanticIdx = 0, uint32_t streamIdx = 0);
    void getDataForIterator(EVertexElementSemantic semantic, uint32_t semanticIdx, uint32_t streamIdx, uint8_t*& data, uint32_t& stride) const;

    uint8_t *getElementData(EVertexElementSemantic semantic, uint32_t semanticIdx = 0) const;
    uint8_t *getStreamData() const;
    uint32_t getIndexElementSize() const;
    uint32_t getElementOffset(EVertexElementSemantic semantic, uint32_t semanticIdx = 0) const;

    uint8_t *getIndexData() const;
    uint16_t *getIndex16() const;
    uint32_t *getIndex32() const;
    uint32_t getVertexCount() const;
    uint32_t getIndexCount() const;
    uint32_t getStreamSize() const;
    uint32_t getIndexBufferOffset() const;
    uint32_t getIndexBufferSize() const;

    EIndexType getIndexType() const { return mIndexType; }

    FVertexDataDesc *getVertexDesc() const;

protected:
    uint32_t getInternalBufferSize() const override;
};