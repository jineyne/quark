#pragma once

#include "CorePrerequisites.h"
#include "Buffer.h"

enum class EIndexType {
    _16bit,
    _32bit,
};

struct FIndexBufferDesc {
    EIndexType indexType;
    uint32_t indexCount;
    EBufferUsage usage = EBufferUsage::Static;
};

class DLL_EXPORT IndexBuffer : public Buffer {
public:
    using Deleter = void(*)(Buffer *);

protected:
    FIndexBufferDesc mDesc;

    Buffer *mBuffer = nullptr;

    Deleter mBufferDeleter = nullptr;

public:
    IndexBuffer(const FIndexBufferDesc &desc);
    virtual ~IndexBuffer();

public:
    static IndexBuffer *New(const FIndexBufferDesc &desc);

public:
    void writeData(uint32_t offset, uint32_t length, const void *src,
                   EBufferWriteType flags = EBufferWriteType::Normal) override;

    EIndexType getIndexType() const { return mDesc.indexType; }
    uint32_t getIndexSize() const;
    uint32_t getIndexCount() const { return mDesc.indexCount; }

protected:
    void *map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) override;
    void unmap() override;
};
