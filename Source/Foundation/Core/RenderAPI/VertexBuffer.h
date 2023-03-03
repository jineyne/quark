#pragma once

#include "CorePrerequisites.h"
#include "Buffer.h"

struct FVertexBufferDesc {
    uint32_t vertexSize;
    uint32_t vertexCount;
    EBufferUsage usage = EBufferUsage::Static;
};

class DLL_EXPORT FVertexBuffer : public FBuffer {
public:
    using Deleter = void(*)(FBuffer *);

protected:
    FVertexBufferDesc mDesc;

    FBuffer *mBuffer = nullptr;

    Deleter mBufferDeleter = nullptr;

public:
    FVertexBuffer(const FVertexBufferDesc &desc);

    virtual ~FVertexBuffer();

public:
    static FVertexBuffer *New(const FVertexBufferDesc &desc);

public:
    void writeData(uint32_t offset, uint32_t length, const void *src,
                   EBufferWriteType flags = EBufferWriteType::Normal) override;

    uint32_t getVertexSize() const { return mDesc.vertexSize; }

    uint32_t getVertexCount() const { return mDesc.vertexCount; }

protected:
    void *map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) override;

    void unmap() override;
};