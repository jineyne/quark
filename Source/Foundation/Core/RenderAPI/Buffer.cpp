#include "Buffer.h"

Buffer::Buffer(uint32_t size, EBufferUsage usage) : mSize(size), mUsage(usage) { }

void *Buffer::lock(uint32_t offset, uint32_t length, const EGpuLockOptions &options) {
    assert(!isLocked() && "Unable to lock buffer. buffer already locked");
    void *result = map(offset, length, options);

    mIsLocked = true;
    return result;
}

void *Buffer::lock(const EGpuLockOptions &options) {
    return lock(0, mSize, options);
}

void Buffer::unlock() {
    assert(isLocked() && "Unable to unlock buffer. buffer not locked");

    unmap();
    mIsLocked = false;
}

void Buffer::writeData(uint32_t offset, uint32_t length, const void *src, EBufferWriteType flags) {
    checkf(offset + length <= mSize, TEXT("Out of Bound!: %ld (offset: %ld + length: %ld) < %ld"), offset + length, offset, length, mSize);

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

void *Buffer::map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) {
    return nullptr;
}

void Buffer::unmap() {

}
