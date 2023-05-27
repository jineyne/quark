#include "GroupAllocator.h"

GroupAllocator::GroupAllocator(GroupAllocator &&rhs) noexcept
        : mData(std::exchange(rhs.mData, nullptr))
        , mCursor(std::exchange(rhs.mCursor, nullptr))
        , mBytesCount(std::exchange(rhs.mBytesCount, 0))  {}

GroupAllocator::~GroupAllocator() {
    if (mBytesCount > 0) {
        q_free(mData);
    }
}

GroupAllocator &GroupAllocator::operator=(GroupAllocator &&rhs) noexcept {
    if (this == &rhs) {
        return *this;
    }

    if (mBytesCount > 0) {
        q_free(mData);
    }

    mData = std::exchange(rhs.mData, nullptr);
    mCursor = std::exchange(rhs.mCursor, nullptr);
    mBytesCount = std::exchange(rhs.mBytesCount, 0);

    return *this;
}


void GroupAllocator::init() {
    assert(mData == nullptr);

    if (mBytesCount > 0) {
        mData = (uint8_t *) q_alloc(mBytesCount);
    }

    mCursor = mData;
}

GroupAllocator & GroupAllocator::reserve(uint32_t amount) {
    assert(mData == nullptr);

    mBytesCount += amount;
    return *this;
}

uint8_t *GroupAllocator::alloc(uint32_t amount) {
    assert(mCursor + amount <= (mData + mBytesCount));

    uint8_t *output = mCursor;
    mCursor += amount;

    return output;
}

void GroupAllocator::free(void *data) {
}

void GroupAllocator::clear() {
    if (mData) {
        q_free(mData);
    }

    mBytesCount = 0;
    mData = nullptr;
    mCursor = nullptr;
}