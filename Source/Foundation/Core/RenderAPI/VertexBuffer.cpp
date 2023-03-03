#include "VertexBuffer.h"
#include "Manager/BufferManager.h"

FVertexBuffer::FVertexBuffer(const FVertexBufferDesc &desc)
        : FBuffer(desc.vertexSize * desc.vertexCount, desc.usage), mDesc(desc) {}

FVertexBuffer::~FVertexBuffer() {
    if (mBuffer) {
        mBufferDeleter(mBuffer);
    }
}

FVertexBuffer *FVertexBuffer::New(const FVertexBufferDesc &desc) {
    return FBufferManager::Instance().createVertexBuffer(desc);
}

void FVertexBuffer::writeData(uint32_t offset, uint32_t length, const void *src, EBufferWriteType flags) {
    mBuffer->writeData(offset, length, src, flags);
}

void *FVertexBuffer::map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) {
    return mBuffer->lock(offset, length, options);
}

void FVertexBuffer::unmap() {
    mBuffer->unlock();
}
