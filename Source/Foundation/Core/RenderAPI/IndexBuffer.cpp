#include "IndexBuffer.h"
#include "Manager/BufferManager.h"

uint32_t calcIndexSize(EIndexType type) {
    switch (type) {
        case EIndexType::_16bit:
            return sizeof(unsigned short);

        default:
        case EIndexType::_32bit:
            return sizeof(unsigned int);
    }
}

IndexBuffer::IndexBuffer(const FIndexBufferDesc &desc)
        : Buffer(calcIndexSize(desc.indexType) * desc.indexCount, desc.usage), mDesc(desc) {}

IndexBuffer::~IndexBuffer() {
    if (mBuffer) {
        mBufferDeleter(mBuffer);
    }
}

IndexBuffer *IndexBuffer::New(const FIndexBufferDesc &desc) {
    return BufferManager::Instance().createIndexBuffer(desc);
}

void IndexBuffer::writeData(uint32_t offset, uint32_t length, const void *src, EBufferWriteType flags) {
    mBuffer->writeData(offset, length, src, flags);
}

uint32_t IndexBuffer::getIndexSize() const {
    return calcIndexSize(mDesc.indexType);
}

void *IndexBuffer::map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) {
    return mBuffer->lock(offset, length, options);
}

void IndexBuffer::unmap() {
    mBuffer->unlock();
}
