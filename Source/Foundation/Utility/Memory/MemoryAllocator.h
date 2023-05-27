#pragma once

#include "Prerequisites/PlatformDefines.h"
#include "Prerequisites/StdHeaders.h"

class MemoryAllocatorBase {
private:
    static uint64_t AllocSize;
    static uint64_t FreeSize;

public:
    static DLL_EXPORT uint64_t GetAllocSize() {
        return AllocSize;
    }

    static DLL_EXPORT uint64_t GetFreeSize() {
        return FreeSize;
    }

    static DLL_EXPORT uint64_t GetAllocatedSize() {
        return AllocSize - FreeSize;
    }

protected:
    static DLL_EXPORT void NotifyAlloc(uint64_t size) {
        AllocSize += size;
    }

    static DLL_EXPORT void NotifyFree(uint64_t size) {
        FreeSize += size;
    }
};

template <typename T>
class TMemoryAllocator : public MemoryAllocatorBase {
public:
    template <typename T = uint8_t>
    static T *Alloc(size_t count = 1) {
        const auto size = sizeof(T) * count;

        NotifyAlloc(size);
        return static_cast<T *>(::malloc(size));
    }

    template <typename T = uint8_t>
    static T *AllocAligned16(size_t count = 1) {
        const auto size = sizeof(T) * count;

        NotifyAlloc(size);
#if PLATFORM == PLATFORM_WIN32
        return static_cast<T *>(::_aligned_malloc(size, 16));
#elif PLATFORM == PLATFORM_LINUX || PLATFORM == PLATFORM_ANDROID
        return static_cast<T *>(::memalign(16, size));
#endif
    }


    template <typename T>
    static void Free(T *ptr, size_t count = 1) {
        if (ptr == nullptr) {
            return;
        }

        const auto size = sizeof(T) * count;

        NotifyFree(size);
        ::free(ptr);
    }

    template <typename T>
    static void FreeAligned16(T *ptr) {
        if (ptr == nullptr) {
            return;
        }

        const auto size = sizeof(T);

        NotifyFree(size);
#if PLATFORM == PLATFORM_WIN32
        ::_aligned_free(ptr);
#elif PLATFORM == PLATFORM_LINUX || PLATFORM == PLATFORM_ANDROID
        ::free(ptr);
#endif
    }

    template <typename T, class ...Args>
    static T *New(Args &&...args) {
        return new (Alloc<T>()) T(std::forward<Args>(args)...);
    }

    template <typename T, class ...Args>
    static T *NewN(size_t count, Args &&...args) {
        T *ptr = (T *) Alloc<T>(count);

        for (auto i = 0; i < count; i++) {
            new (&ptr[i]) T(std::forward<Args>(args)...);
        }

        return ptr;
    }

    template <typename T>
    static void Delete(T *ptr) {
        if (ptr == nullptr) {
            return;
        }

        ptr->~T();
        Free(ptr);
    }

    template <typename T>
    static void DeleteN(T *ptr, size_t count) {
        if (ptr == nullptr) {
            return;
        }

        for (auto i = 0; i < count; i++) {
            ptr[i].~T();
        }

        Free(ptr, count);
    }
};

class GenAlloc {};

inline void* q_alloc(size_t count) {
    return TMemoryAllocator<GenAlloc>::Alloc(count);
}

template<class T>
T* q_alloc(size_t count = 1) {
    return (T*) TMemoryAllocator<GenAlloc>::Alloc(sizeof(T) * count);
}

template<class T>
void q_free(T *ptr) {
    TMemoryAllocator<GenAlloc>::Free(ptr);
}

template<typename T, class ...Args>
static T *q_new(Args &&... args) {
    return TMemoryAllocator<GenAlloc>::New<T, Args...>(std::forward<Args>(args)...);
}

template<class T>
T* q_newN(size_t count) {
    return TMemoryAllocator<GenAlloc>::NewN<T>(count);
}

template<class T>
void q_delete(T *ptr) {
    TMemoryAllocator<GenAlloc>::Delete<T>(ptr);
}

template<class T>
void q_deleteN(T *ptr, const size_t num) {
    TMemoryAllocator<GenAlloc>::DeleteN<T>(ptr, num);
}

template<typename T>
class StdAllocator {
public:
    using value_type = T;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    constexpr StdAllocator() = default;

    constexpr StdAllocator(StdAllocator &&) = default;

    constexpr StdAllocator(const StdAllocator &) = default;

    template<class U>
    constexpr StdAllocator(const StdAllocator<U> &) {}

    template<class U>
    constexpr bool operator==(const StdAllocator<U> &) const noexcept {
        return true;
    }

    template<class U>
    constexpr bool operator!=(const StdAllocator<U> &) const noexcept {
        return false;
    }

    StdAllocator<T> &operator =(const StdAllocator<T> &rhs) noexcept = default; // constexpr

    template<class U>
    class rebind {
    public:
        using other = StdAllocator<U>;
    };

    static T *allocate(const size_t num) {
        if (num == 0) {
            return nullptr;
        }

        if (num > max_size()) {
            return nullptr;
        }

        void *const pv = TMemoryAllocator<GenAlloc>::Alloc<T>(num);
        if (!pv) {
            return nullptr;
        }

        return static_cast<T *>(pv);
    }

    static void deallocate(pointer p, size_type) {
        TMemoryAllocator<GenAlloc>::Free(p);
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

template<class T>
struct Deleter {
    constexpr Deleter() noexcept = default;

    template<class T2, typename std::enable_if<std::is_convertible<T2 *, T *>::value, int>::type = 0>
    constexpr Deleter(const Deleter<T2> &other) noexcept {}

    void operator()(T *ptr) const {
        TMemoryAllocator<GenAlloc>::Delete(ptr);
    }
};