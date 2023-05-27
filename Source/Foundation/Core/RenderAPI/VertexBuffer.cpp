#include "VertexBuffer.h"
#include "Manager/BufferManager.h"

VertexBuffer::VertexBuffer(const VertexBufferDesc &desc)
        : Buffer(desc.vertexSize * desc.vertexCount, desc.usage), mDesc(desc) {}

VertexBuffer::~VertexBuffer() {
    if (mBuffer) {
        mBufferDeleter(mBuffer);
    }
}

VertexBuffer *VertexBuffer::New(const VertexBufferDesc &desc) {
    return BufferManager::Instance().createVertexBuffer(desc);
}

void VertexBuffer::writeData(uint32_t offset, uint32_t length, const void *src, EBufferWriteType flags) {
    mBuffer->writeData(offset, length, src, flags);
}

void *VertexBuffer::map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) {
    return mBuffer->lock(offset, length, options);
}

void VertexBuffer::unmap() {
    mBuffer->unlock();
}
