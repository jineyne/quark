#include "GpuBuffer.h"
#include "Misc/Module.h"
#include "Manager/BufferManager.h"

uint32_t getBufferSize(const FGpuBufferDesc &desc) {
    uint32_t elementSize;

    if (desc.type == EBufferType::Standard)
        elementSize = FGpuBuffer::GetFormatSize(desc.format);
    else
        elementSize = desc.elementSize;

    return elementSize * desc.elementCount;
}

FGpuBuffer::FGpuBuffer(const FGpuBufferDesc &desc) : FBuffer(getBufferSize(desc), desc.usage), mDesc(desc) {
    if (desc.type != EBufferType::Standard) {
        assert(desc.format == EGpuBufferFormat::Unknown &&
               "Format must be set to EGpuBufferFormat::Unknown when using non-standard buffers");
    } else {
        assert(desc.elementSize == 0 &&
               "No element size can be provided for standard buffer. Size is determined from format.");
    }
}

FGpuBuffer::~FGpuBuffer() {
    if (mBuffer) {
        mBufferDeleter(mBuffer);
    }
}

FGpuBuffer *FGpuBuffer::New(const FGpuBufferDesc &desc) {
    return FBufferManager::Instance().createGpuBuffer(desc);
}

uint32_t FGpuBuffer::GetFormatSize(EGpuBufferFormat format) {
    static bool lookupInitialized = false;

    constexpr const uint32_t size = static_cast<uint32_t>(EGpuBufferFormat::Count);

    static uint32_t lookup[size];
    if (!lookupInitialized) {
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_16x1f)] = 2;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_16x2f)] = 4;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_16x4f)] = 8;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_32x1f)] = 4;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_32x2f)] = 8;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_32x3f)] = 12;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_32x4f)] = 16;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_8x1)] = 1;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_8x2)] = 2;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_8x4)] = 4;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_16x1)] = 2;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_16x2)] = 4;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_16x4)] = 8;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_8x1s)] = 1;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_8x2s)] = 2;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_8x4s)] = 4;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_16x1s)] = 2;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_16x2s)] = 4;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_16x4s)] = 8;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_32x1s)] = 4;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_32x2s)] = 8;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_32x3s)] = 12;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_32x4s)] = 16;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_8x1u)] = 1;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_8x2u)] = 2;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_8x4u)] = 4;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_16x1u)] = 2;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_16x2u)] = 4;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_16x4u)] = 8;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_32x1u)] = 4;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_32x2u)] = 8;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_32x3u)] = 12;
        lookup[static_cast<uint32_t>(EGpuBufferFormat::_32x4u)] = 16;

        lookupInitialized = true;
    }

    auto fmt = static_cast<uint32_t>(format);
    if (fmt >= size) {
        return 0;
    }

    return lookup[fmt];
}

void FGpuBuffer::writeData(uint32_t offset, uint32_t length, const void *src, EBufferWriteType flags) {
    mBuffer->writeData(offset, length, src, flags);
}

void *FGpuBuffer::map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) {
    return mBuffer->lock(offset, length, options);
}

void FGpuBuffer::unmap() {
    mBuffer->unlock();
}
