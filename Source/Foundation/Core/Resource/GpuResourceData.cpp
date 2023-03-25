#include "GpuResourceData.h"

void verifyLockAndThread(const FGpuResourceData *data) {
    if(data->isLocked()) {
        // EXCEPT?
    }
}

FGpuResourceData::FGpuResourceData(const FGpuResourceData &copy) {
    mData = copy.mData;
    mLocked = copy.mLocked;
    mOwnsData = false;
}

FGpuResourceData::~FGpuResourceData() {
    freeInternalBuffer();
}

FGpuResourceData &FGpuResourceData::operator=(const FGpuResourceData &rhs) {
    mData = rhs.mData;
    mLocked = rhs.mLocked;
    mOwnsData = false;

    return *this;
}

uint8_t *FGpuResourceData::getData() const {
    verifyLockAndThread(this);
    return mData;
}

void FGpuResourceData::setData(std::unique_ptr<uint8_t[]> &data) {
    verifyLockAndThread(this);
    freeInternalBuffer();

    mData = data.release();
    mOwnsData = true;
}

void FGpuResourceData::allocateInternalBuffer() {
    allocateInternalBuffer(getInternalBufferSize());
}

void FGpuResourceData::allocateInternalBuffer(size_t size) {
    mData = (uint8_t *) malloc(size);
    mOwnsData = true;
}

void FGpuResourceData::freeInternalBuffer() {
    if (mData == nullptr || !mOwnsData) {
        return;
    }

    q_free(mData);
    mData = nullptr;
}

void FGpuResourceData::setExternalBuffer(uint8_t *data) {
    freeInternalBuffer();

    mData = data;
    mOwnsData = false;
}

bool FGpuResourceData::isLocked() const {
    return mLocked;
}

void FGpuResourceData::lock() const {
    mLocked = true;
}

void FGpuResourceData::unlock() const {
    mLocked = false;
}
