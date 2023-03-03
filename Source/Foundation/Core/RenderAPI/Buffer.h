#pragma once

#include "CorePrerequisites.h"

enum class EBufferUsage {
    Static = 1 << 0,
    Dynamic = 1 << 1,
    LoadStore = Static | 1 << 2,
};

ENUM_CLASS_FLAGS(EBufferUsage)

enum class EBufferType {
    Standard,
    Structured,
    InDirectArgument,
};

ENUM_CLASS_FLAGS(EBufferType)

enum class EBufferWriteType {
    Normal,
    Discard,
    NoOverWrite
};

ENUM_CLASS_FLAGS(EBufferWriteType)

enum class EGpuLockOptions {
    ReadWrite,
    WriteOnlyDiscard,
    WriteOnlyDiscardRange,
    ReadOnly,
    WriteOnlyNoOverWrite,
    WriteOnly,
};

ENUM_CLASS_FLAGS(EGpuLockOptions)

class DLL_EXPORT FBuffer {
protected:
    uint32_t mSize;
    EBufferUsage mUsage;
    bool mIsLocked = false;

public:
    FBuffer(uint32_t size, EBufferUsage usage);

    virtual ~FBuffer() = default;

public:
    virtual void *lock(uint32_t offset, uint32_t length, const EGpuLockOptions &options);
    void *lock(const EGpuLockOptions &options);

    virtual void unlock();

    virtual void writeData(uint32_t offset, uint32_t length, const void *src, EBufferWriteType flags = EBufferWriteType::Normal);

    uint32_t getSize() const { return mSize; }
    bool isLocked() const { return mIsLocked; }

protected:
    virtual void *map(uint32_t offset, uint32_t length, const EGpuLockOptions &options);

    virtual void unmap();
};