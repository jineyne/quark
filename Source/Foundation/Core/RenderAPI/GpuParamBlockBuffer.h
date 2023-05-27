#pragma once

#include "CorePrerequisites.h"
#include "Buffer.h"

class DLL_EXPORT GpuParamBlockBuffer {
protected:
    Buffer *mBuffer = nullptr;
    EBufferUsage mUsage;

    uint32_t mSize = 0;
    uint8_t *mCachedData = nullptr;

    bool mGPUBufferDirty = false;

public:
    GpuParamBlockBuffer(uint32_t size, EBufferUsage usage);
    virtual ~GpuParamBlockBuffer();

public:
    static GpuParamBlockBuffer *New(uint32_t size, EBufferUsage usage = EBufferUsage::Dynamic);

public:
    void writeToGPU(const uint8_t *data);
    void flushToGPU();
    void write(uint32_t offset, const void *data, uint32_t size);
    void read(uint32_t offset, void *data, uint32_t size);
    void zeroOut(uint32_t offset, uint32_t size);

    uint32_t getSize() const { return mSize; }
};
