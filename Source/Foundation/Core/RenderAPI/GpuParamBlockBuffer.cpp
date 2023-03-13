#include "GpuParamBlockBuffer.h"
#include "Manager/BufferManager.h"
#include "RenderAPI/RenderAPI.h"

FGpuParamBlockBuffer::FGpuParamBlockBuffer(uint32_t size, EBufferUsage usage) : mSize(size), mUsage(usage) {
    if (mSize > 0) {
        mCachedData = (uint8_t *) malloc(mSize);
        std::memset(mCachedData, 0, mSize);
    }
}

FGpuParamBlockBuffer::~FGpuParamBlockBuffer() {
    if (mCachedData != nullptr) {
        free(mCachedData);
    }
}

FGpuParamBlockBuffer *FGpuParamBlockBuffer::New(uint32_t size, EBufferUsage usage) {
    return FBufferManager::Instance().createGpuParamBlockBuffer(size, usage);
}

void FGpuParamBlockBuffer::writeToGPU(const uint8_t *data) {
    mBuffer->writeData(0, mSize, data, EBufferWriteType::Discard);
}

void FGpuParamBlockBuffer::flushToGPU() {
    if (mGPUBufferDirty) {
        writeToGPU(mCachedData);
        mGPUBufferDirty = false;
    }
}

void FGpuParamBlockBuffer::write(uint32_t offset, const void *data, uint32_t size) {
#if DEBUG_MODE
    if ((offset + size) > mSize) {
        EXCEPT(FLogRenderAPI, InvalidParametersException,
               TEXT("Wanted range is out of buffer bounds. vailable range: 0 .. %ld. Wanted range: %ld .. %ld."), mSize, offset, offset + size);
    }
#endif
    std::memcpy(mCachedData + offset, data, size);
    mGPUBufferDirty = true;
}

void FGpuParamBlockBuffer::read(uint32_t offset, void *data, uint32_t size) {
#if DEBUG_MODE
    if ((offset + size) > mSize) {
        EXCEPT(FLogRenderAPI, InvalidParametersException,
               TEXT("Wanted range is out of buffer bounds. vailable range: 0 .. %ld. Wanted range: %ld .. %ld."), mSize, offset, offset + size);
    }
#endif
    std::memcpy(data, mCachedData + offset, size);
}

void FGpuParamBlockBuffer::zeroOut(uint32_t offset, uint32_t size) {
#if DEBUG_MODE
    if ((offset + size) > mSize) {
        EXCEPT(FLogRenderAPI, InvalidParametersException,
               TEXT("Wanted range is out of buffer bounds. vailable range: 0 .. %ld. Wanted range: %ld .. %ld."), mSize, offset, offset + size);
    }
#endif
    std::memset(mCachedData + offset, 0, size);
    mGPUBufferDirty = true;
}


