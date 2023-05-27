#include "GpuResourceData.h"

void verifyLockAndThread(const GpuResourceData *data) {
    if(data->isLocked()) {
        // EXCEPT?
    }
}

GpuResourceData::GpuResourceData(const GpuResourceData &copy) {
    mData = copy.mData;
    mLocked = copy.mLocked;
    mOwnsData = false;
}

GpuResourceData::~GpuResourceData() {
    freeInternalBuffer();
}

GpuResourceData &GpuResourceData::operator=(const GpuResourceData &rhs) {
    mData = rhs.mData;
    mLocked = rhs.mLocked;
    mOwnsData = false;

    return *this;
}

uint8_t *GpuResourceData::getData() const {
    verifyLockAndThread(this);
    return mData;
}

void GpuResourceData::setData(std::unique_ptr<uint8_t[]> &data) {
    verifyLockAndThread(this);
    freeInternalBuffer();

    mData = data.release();
    mOwnsData = true;
}

void GpuResourceData::allocateInternalBuffer() {
    allocateInternalBuffer(getInternalBufferSize());
}

void GpuResourceData::allocateInternalBuffer(size_t size) {
    mData = (uint8_t *) malloc(size);
    mOwnsData = true;
}

void GpuResourceData::freeInternalBuffer() {
    if (mData == nullptr || !mOwnsData) {
        return;
    }

    q_free(mData);
    mData = nullptr;
}

void GpuResourceData::setExternalBuffer(uint8_t *data) {
    freeInternalBuffer();

    mData = data;
    mOwnsData = false;
}

bool GpuResourceData::isLocked() const {
    return mLocked;
}

void GpuResourceData::lock() const {
    mLocked = true;
}

void GpuResourceData::unlock() const {
    mLocked = false;
}
