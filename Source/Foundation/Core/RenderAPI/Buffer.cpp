#include "Buffer.h"

FBuffer::FBuffer(uint32_t size, EBufferUsage usage) : mSize(size), mUsage(usage) { }

void *FBuffer::lock(uint32_t offset, uint32_t length, const EGpuLockOptions &options) {
    assert(!isLocked() && "Unable to lock buffer. buffer already locked");
    void *result = map(offset, length, options);

    mIsLocked = true;
    return result;
}

void *FBuffer::lock(const EGpuLockOptions &options) {
    return lock(0, mSize, options);
}

void FBuffer::unlock() {
    assert(isLocked() && "Unable to unlock buffer. buffer not locked");

    unmap();
    mIsLocked = false;
}

void FBuffer::writeData(uint32_t offset, uint32_t length, const void *src, EBufferWriteType flags) {
    EGpuLockOptions lockOptions = EGpuLockOptions::WriteOnly;
    if (flags == EBufferWriteType::Discard) {
        lockOptions = EGpuLockOptions::WriteOnlyDiscard;
    } else if (flags == EBufferWriteType::NoOverWrite) {
        lockOptions = EGpuLockOptions::WriteOnlyNoOverWrite;
    }

    void *data = lock(offset, length, lockOptions);
    memcpy(data, src, length);
    unlock();
}

void *FBuffer::map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) {
    return nullptr;
}

void FBuffer::unmap() {

}
