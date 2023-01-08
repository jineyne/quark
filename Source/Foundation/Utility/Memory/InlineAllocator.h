#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "HeapAllocator.h"

template <typename T, int32_t DefaultSize = 1024, typename InternalAllocator = THeapAllocator<T>>
class TInlineAllocator {
private:
    InternalAllocator mInternalAllocator;
    T mInlineData[DefaultSize];

public:
    TInlineAllocator() {
    }

public:
    FORCEINLINE void resize(size_t prevCount, size_t count, size_t elementSize) {
        if (count <= DefaultSize) {
            if (mInternalAllocator.getAllocation()) {
                std::memmove(mInlineData, (T *) mInternalAllocator.getAllocation(), sizeof(T) * prevCount);
                mInternalAllocator.resize(0, 0, elementSize);
            }
        } else {
            if (mInternalAllocator.getAllocation()) {
                mInternalAllocator.resize(0, count, elementSize);
                std::memmove((T *) mInternalAllocator.getAllocation(), getInlineElements(), sizeof(T) * prevCount);
            } else {
                mInternalAllocator.resize(prevCount, count, elementSize);
            }
        }
    }

    FORCEINLINE T *getAllocation() {
        if (mInternalAllocator.hasAllocation()) {
            return mInternalAllocator.getAllocation();
        } else {
            return mInlineData;
        }
    }

    T *getInlineElements() {
        return mInlineData;
    }
};