#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

class GroupAllocator {
private:
    uint8_t *mData = nullptr;
    uint8_t *mCursor = nullptr;
    uint32_t mBytesCount = 0;

public:
    GroupAllocator() = default;
    GroupAllocator(GroupAllocator &&rhs) noexcept;
    ~GroupAllocator();

public:
    GroupAllocator &operator=(GroupAllocator &&rhs) noexcept;

public:
    void init();

    GroupAllocator &reserve(uint32_t amount);

    template <typename T>
    GroupAllocator &reserve(uint32_t count) {
        return reserve(sizeof(T) * count);
    }

    uint8_t *alloc(uint32_t amount);

    template <typename T>
    T *alloc(uint32_t count = 1) {
        return (T *) q_alloc(sizeof(T) * count);
    }

    void free(void *ptr);

    void clear();

    template <typename T>
    T *construct(uint32_t count = 1) {
        T *data = static_cast<T *>(q_alloc(sizeof(T) * count));

        for (auto i = 0; i < count; i ++) {
            new ((void *) &data[i]) T;
        }

        return data;
    }

    template <typename T, class...Args>
    T *construct(Args &&...args, uint32_t count = 1) {
        T *data = static_cast<T *>(q_alloc(sizeof(T) * count));

        for (auto i = 0; i < count; i++) {
            new ((void *) &data[i]) T (std::forward<Args>(args)...);
        }

        return data;
    }

    template <typename T>
    void destruct(T *ptr) {
        ptr->~T();

        q_free(ptr);
    }

    template <typename T>
    void destruct(T *ptr, uint32_t count) {
        for (auto i = 0; i < count; i++) {
            ptr[i]->~T();
        }

        q_free(ptr);
    }
};