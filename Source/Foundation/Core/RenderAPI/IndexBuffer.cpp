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

FIndexBuffer::FIndexBuffer(const FIndexBufferDesc &desc)
        : FBuffer(calcIndexSize(desc.indexType) * desc.indexCount, desc.usage), mDesc(desc) {}

FIndexBuffer::~FIndexBuffer() {
    if (mBuffer) {
        mBufferDeleter(mBuffer);
    }
}

FIndexBuffer *FIndexBuffer::New(const FIndexBufferDesc &desc) {
    return FBufferManager::Instance().createIndexBuffer(desc);
}

void FIndexBuffer::writeData(uint32_t offset, uint32_t length, const void *src, EBufferWriteType flags) {
    mBuffer->writeData(offset, length, src, flags);
}

uint32_t FIndexBuffer::getIndexSize() const {
    return calcIndexSize(mDesc.indexType);
}

void *FIndexBuffer::map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) {
    return mBuffer->lock(offset, length, options);
}

void FIndexBuffer::unmap() {
    mBuffer->unlock();
}
