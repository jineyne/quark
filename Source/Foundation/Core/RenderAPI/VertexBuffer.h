#pragma once

#include "CorePrerequisites.h"
#include "Buffer.h"

struct VertexBufferDesc {
    uint32_t vertexSize;
    uint32_t vertexCount;
    EBufferUsage usage = EBufferUsage::Static;
};

class DLL_EXPORT VertexBuffer : public Buffer {
public:
    using Deleter = void(*)(Buffer *);

protected:
    VertexBufferDesc mDesc;

    Buffer *mBuffer = nullptr;

    Deleter mBufferDeleter = nullptr;

public:
    VertexBuffer(const VertexBufferDesc &desc);

    virtual ~VertexBuffer();

public:
    static VertexBuffer *New(const VertexBufferDesc &desc);

public:
    void writeData(uint32_t offset, uint32_t length, const void *src,
                   EBufferWriteType flags = EBufferWriteType::Normal) override;

    uint32_t getVertexSize() const { return mDesc.vertexSize; }

    uint32_t getVertexCount() const { return mDesc.vertexCount; }

protected:
    void *map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) override;

    void unmap() override;
};