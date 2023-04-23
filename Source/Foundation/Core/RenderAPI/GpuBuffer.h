#pragma once

#include "CorePrerequisites.h"
#include "Buffer.h"

enum class EGpuBufferFormat {
    _16x1f,
    _16x2f,
    _16x4f,

    _32x1f,
    _32x2f,
    _32x3f,
    _32x4f,

    _8x1,
    _8x2,
    _8x4,

    _16x1,
    _16x2,
    _16x4,

    _8x1s,
    _8x2s,
    _8x4s,
    _16x1s,
    _16x2s,
    _16x4s,
    _32x1s,
    _32x2s,
    _32x3s,
    _32x4s,

    _8x1u,
    _8x2u,
    _8x4u,
    _16x1u,
    _16x2u,
    _16x4u,
    _32x1u,
    _32x2u,
    _32x3u,
    _32x4u,

    Count,
    Unknown,
};

struct FGpuBufferDesc {
    uint32_t elementCount = 0;
    uint32_t elementSize = 0;

    EBufferType type = EBufferType::Standard;
    EBufferUsage usage = EBufferUsage::Static;
    EGpuBufferFormat format = EGpuBufferFormat::_32x4f;
};

class DLL_EXPORT FGpuBuffer : public FBuffer {
public:
    using Deleter = void(*)(FBuffer *);

protected:
    FGpuBufferDesc mDesc;

    FBuffer *mBuffer = nullptr;

    Deleter mBufferDeleter = nullptr;

public:
    FGpuBuffer(const FGpuBufferDesc &desc);
    virtual ~FGpuBuffer();

public:
    static FGpuBuffer *New(const FGpuBufferDesc &desc);
    static uint32_t GetFormatSize(EGpuBufferFormat format);

public:
    void writeData(uint32_t offset, uint32_t length, const void *src, EBufferWriteType flags) override;

    const FGpuBufferDesc &getDesc() const { return mDesc; }

protected:
    void *map(uint32_t offset, uint32_t length, const EGpuLockOptions &options) override;
    void unmap() override;
};

