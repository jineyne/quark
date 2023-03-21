#pragma once

#include "CorePrerequisites.h"

class DLL_EXPORT FGpuResourceData {
private:
    uint8_t *mData = nullptr;

    bool mOwnsData = false;
    mutable bool mLocked = false;

public:
    FGpuResourceData() = default;
    FGpuResourceData(const FGpuResourceData &copy);

    virtual ~FGpuResourceData();

public:
    FGpuResourceData &operator=(const FGpuResourceData &rhs);

public:
    uint8_t *getData() const;
    void setData(std::unique_ptr<uint8_t[]> &data);

    void allocateInternalBuffer();
    void allocateInternalBuffer(size_t size);
    void freeInternalBuffer();

    void setExternalBuffer(uint8_t *data);

    bool isLocked() const;
    void lock() const;
    void unlock() const;

protected:
    virtual uint32_t getInternalBufferSize() const = 0;
};