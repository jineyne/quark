#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

template <typename T>
class THeapAllocator {
private:
    size_t mCapacity;
    T *mData = nullptr;

public:
    THeapAllocator() : mData(nullptr) { }

    ~THeapAllocator() {
        if (mData) {
            ::free(mData);
        }
    }

public:
    template <class SrcAllocator>
    FORCEINLINE void moveToEmpty(SrcAllocator &other) {
        check(((void *) this) != ((void *) other));
        if (mData) {
            ::free(mData);
        }

        mData = other.mDesc;
        other.mDesc = nullptr;
    }

    FORCEINLINE T *getAllocation() const { return mData; }
    FORCEINLINE size_t capacity() const { return mCapacity; }

    FORCEINLINE void resize(size_t prevCount, size_t count, size_t elementSize = sizeof(T)) {
        if (mData || count) {
            mCapacity = count * elementSize;
            mData = (T *) ::realloc(mData, mCapacity);
        }
    }

    FORCEINLINE bool hasAllocation() const { return !!mData; }
};