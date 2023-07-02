#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

class DLL_EXPORT FixedSizeElementAllocator {
private:
    size_t mElementSize = 0;

public:
    FixedSizeElementAllocator() = default;
    FixedSizeElementAllocator(FixedSizeElementAllocator &&rhs) noexcept;
    ~FixedSizeElementAllocator();

public:
    FixedSizeElementAllocator &operator=(FixedSizeElementAllocator &&rhs) noexcept;

public:
    void init(size_t elementSize);

    void *alloc(uint32_t amount);
    void free(void *ptr);

    template <typename T, class...Args>
    void *construct(Args &&...args, uint32_t count = 1) {
        T *data = static_cast<T *>(q_alloc(sizeof(T) * count));

        for (auto i = 0; i < count; i++) {
            new ((void *) &data[i]) T (std::forward<Args>(args)...);
        }

        return data;
    }
};

template<typename T>
class StdFixedSizeElementAllocator {
public:
    using value_type = T;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    static FixedSizeElementAllocator Internal;

    constexpr StdFixedSizeElementAllocator() = default;

    constexpr StdFixedSizeElementAllocator(StdFixedSizeElementAllocator &&) = default;

    constexpr StdFixedSizeElementAllocator(const StdFixedSizeElementAllocator &) = default;

    template<class U>
    constexpr StdFixedSizeElementAllocator(const StdFixedSizeElementAllocator<U> &) {}

    template<class U>
    constexpr bool operator==(const StdFixedSizeElementAllocator<U> &) const noexcept {
        return true;
    }

    template<class U>
    constexpr bool operator!=(const StdFixedSizeElementAllocator<U> &) const noexcept {
        return false;
    }

    StdFixedSizeElementAllocator<T> &operator =(const StdFixedSizeElementAllocator<T> &rhs) noexcept = default; // constexpr

    template<class U>
    class rebind {
    public:
        using other = StdFixedSizeElementAllocator<U>;
    };

    static T *allocate(const size_t num) {
        if (num == 0) {
            return nullptr;
        }

        if (num > max_size()) {
            return nullptr;
        }

        void *const pv = Internal.alloc(num);
        if (!pv) {
            return nullptr;
        }

        return static_cast<T *>(pv);
    }

    static void deallocate(pointer p, size_type) {
        Internal.free(p);
    }

    static size_t max_size() {  // constexpr
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    static void destroy(pointer p) { p->~T(); } // constexpr

    template<class... Args>
    static void construct(pointer p, Args &&... args) {
        new(p) T(std::forward<Args>(args)...);
    }
};

template <typename T>
FixedSizeElementAllocator StdFixedSizeElementAllocator<T>::Internal;