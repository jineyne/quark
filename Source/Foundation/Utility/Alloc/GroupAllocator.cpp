//
// Created by jiney on 2023-03-07.
//

#include "GroupAllocator.h"

FGroupAllocator::FGroupAllocator(FGroupAllocator &&rhs) noexcept
        : mData(std::exchange(rhs.mData, nullptr))
        , mCursor(std::exchange(rhs.mCursor, nullptr))
        , mBytesCount(std::exchange(rhs.mBytesCount, 0))  {}

FGroupAllocator::~FGroupAllocator() {
    if (mBytesCount > 0) {
        free(mData);
    }
}

FGroupAllocator &FGroupAllocator::operator=(FGroupAllocator &&rhs) noexcept {
    if (this == &rhs) {
        return *this;
    }

    if (mBytesCount > 0) {
        free(mData);
    }

    mData = std::exchange(rhs.mData, nullptr);
    mCursor = std::exchange(rhs.mCursor, nullptr);
    mBytesCount = std::exchange(rhs.mBytesCount, 0);

    return *this;
}


void FGroupAllocator::init() {
    assert(mData == nullptr);

    if (mBytesCount > 0) {
        mData = (uint8_t *) malloc(mBytesCount);
    }

    mCursor = mData;
}

FGroupAllocator & FGroupAllocator::reserve(uint32_t amount) {
    assert(mData == nullptr);

    mBytesCount += amount;
    return *this;
}

uint8_t *FGroupAllocator::alloc(uint32_t amount) {
    assert(mCursor + amount <= (mData + mBytesCount));

    uint8_t *output = mCursor;
    mCursor += amount;

    return output;
}

void FGroupAllocator::free(void *data) {
}

void FGroupAllocator::clear() {
    if (mData) {
        free(mData);
    }

    mBytesCount = 0;
    mData = nullptr;
    mCursor = nullptr;
}