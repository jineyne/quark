#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

class FGroupAllocator {
private:
    uint8_t *mData = nullptr;
    uint8_t *mCursor = nullptr;
    uint32_t mBytesCount = 0;

public:
    FGroupAllocator() = default;
    FGroupAllocator(FGroupAllocator &&rhs) noexcept;
    ~FGroupAllocator();

public:
    FGroupAllocator &operator=(FGroupAllocator &&rhs) noexcept;

public:
    void init();

    FGroupAllocator &reserve(uint32_t amount);

    template <typename T>
    FGroupAllocator &reserve(uint32_t count) {
        return reserve(sizeof(T) * count);
    }

    uint8_t *alloc(uint32_t amount);

    template <typename T>
    T *alloc(uint32_t count = 1) {
        return (T *) malloc(sizeof(T) * count);
    }

    void free(void *ptr);

    void clear();

    template <typename T>
    T *construct(uint32_t count = 1) {
        T *data = static_cast<T *>(malloc(sizeof(T) * count));

        for (auto i = 0; i < count; i ++) {
            new ((void *) &data[i]) T;
        }

        return data;
    }

    template <typename T, class...Args>
    T *construct(Args &&...args, uint32_t count = 1) {
        T *data = static_cast<T *>(malloc(sizeof(T) * count));

        for (auto i = 0; i < count; i++) {
            new ((void *) &data[i]) T (std::forward<Args>(args)...);
        }

        return data;
    }

    template <typename T>
    void destruct(T *ptr) {
        ptr->~T();

        free(ptr);
    }

    template <typename T>
    void destruct(T *ptr, uint32_t count) {
        for (auto i = 0; i < count; i++) {
            ptr[i]->~T();
        }

        free(ptr);
    }
};