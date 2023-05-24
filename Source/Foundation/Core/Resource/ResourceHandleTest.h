#pragma once

#include "CorePrerequisites.h"
#include "Misc/UUID.h"
#include "ResourceHandleTest.g.h"

QSTRUCT()
struct DLL_EXPORT FResourceHandleData {
GENERATED_BODY()

    class FResource *ptr;

    QPROPERTY()
    FUuid uuid;

    bool isLoaded = false;
    std::atomic<std::uint32_t> refCount{0};
};

QCLASS()
class DLL_EXPORT FResourceHandleBase {
    GENERATED_BODY()

protected:
    FResourceHandleData *mData = nullptr;
    bool bIsCreated = false;

    bool bIsBuiltin = false;

public:
    bool isLoaded(bool checkDependencies = true) const;
    void blockUntilLoaded(bool waitForDependencies = true) const;

    void release();

    void setIsBuiltin() { bIsBuiltin = true; }

    const FUuid &getUUID() const { return mData != nullptr ? mData->uuid : FUuid::Empty; }
    FResourceHandleData *getHandleData() const { return mData; }

protected:
    void destroy();

    void setHandleData(FResource *ptr, const FUuid &uuid);
    void clearHandleData();

    void addInternalRef();
    void removeInternalRef();

    void notifyLoadComplete();

    void throwIfNotLoaded() const;

private:
    friend class FResources;
};

template<bool WeakHandle>
class DLL_EXPORT TResourceHandleBase : public FResourceHandleBase {
};

template<>
class DLL_EXPORT TResourceHandleBase<true> : public FResourceHandleBase {
protected:
void addRef() {};
void releaseRef() {};
};

template<>
class DLL_EXPORT TResourceHandleBase<false> : public FResourceHandleBase {
protected:
void addRef() {
    if (mData) {
        mData->refCount.fetch_add(1, std::memory_order_relaxed);
    }
};

void releaseRef() {
    if (mData) {
        std::uint32_t refCount = mData->refCount.fetch_sub(1, std::memory_order_release);

        if (refCount == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            destroy();
        }
    }
};
};

template<typename T, bool WeakHandle>
class TResourceHandle : public TResourceHandleBase<WeakHandle> {
public:
    TResourceHandle() = default;

    TResourceHandle(std::nullptr_t) {}

    /**	Copy constructor. */
    TResourceHandle(const TResourceHandle &other) {
        this->mData = other.getHandleData();
        this->addRef();
    }

    /** Move constructor. */
    TResourceHandle(TResourceHandle &&other) {
        this->mData = other.getHandleData();
        this->addRef();
    }

    ~TResourceHandle() {
        this->releaseRef();
    }

    /**	Converts a specific handle to generic Resource handle. */
    operator TResourceHandle<FResource, WeakHandle>() const {
        TResourceHandle<FResource, WeakHandle> handle;
        handle.setHandleData(this->getHandleData());

        return handle;
    }

    /**
     * Returns internal resource pointer.
     *
     * @note	Throws exception if handle is invalid.
     */
    T *operator->() const { return get(); }

    /**
     * Returns internal resource pointer and dereferences it.
     *
     * @note	Throws exception if handle is invalid.
     */
    T &operator*() const { return *get(); }

    /** Clears the handle making it invalid and releases any references held to the resource. */
    TResourceHandle<T, WeakHandle> &operator=(std::nullptr_t ptr) {
        this->releaseRef();
        this->mData = nullptr;

        return *this;
    }

    /**	Copy assignment. */
    TResourceHandle<T, WeakHandle> &operator=(const TResourceHandle<T, WeakHandle> &rhs) {
        setHandleData(rhs.getHandleData());
        return *this;
    }

    /**	Move assignment. */
    TResourceHandle &operator=(TResourceHandle &&other) {
        if (this == &other)
            return *this;

        this->releaseRef();
        this->mData = std::exchange(other.mData, nullptr);

        return *this;
    }

    template<class _Ty>
    struct Bool_struct {
        int _Member;
    };

    /**
     * Allows direct conversion of handle to bool.
     *
     * @note	This is needed because we can't directly convert to bool since then we can assign pointer to bool and
     *			that's weird.
     */
    operator int Bool_struct<T>::*() const {
        return ((this->mData != nullptr && !this->mData->uuid.empty()) ? &Bool_struct<T>::_Member : 0);
    }

    /**
     * Returns internal resource pointer and dereferences it.
     *
     * @note	Throws exception if handle is invalid.
     */
    T *get() const {
        this->throwIfNotLoaded();

        return reinterpret_cast<T *>(this->mData->ptr);
    }

    /**
     * Returns the internal shared pointer to the resource.
     *
     * @note	Throws exception if handle is invalid.
     */
    T *getInternalPtr() const {
        this->throwIfNotLoaded();

        return static_cast<T *>(this->mData->ptr);
    }

    /** Converts a handle into a weak handle. */
    TResourceHandle<T, true> getWeak() const {
        TResourceHandle<T, true> handle;
        handle.setHandleData(this->getHandleData());

        return handle;
    }

protected:
    friend class FResources;

    template<class _T, bool _Weak>
    friend class TResourceHandle;

    template<class _Ty1, class _Ty2, bool _Weak2, bool _Weak1>
    friend TResourceHandle<_Ty1, _Weak1> StaticResourceCast(const TResourceHandle<_Ty2, _Weak2> &other);

    template<class _Ty1, class _Ty2, bool _Weak2>
    friend TResourceHandle<_Ty1, false> StaticResourceCast(const TResourceHandle<_Ty2, _Weak2> &other);

    /**
     * Constructs a new valid handle for the provided resource with the provided UUID.
     *
     * @note	Handle will take ownership of the provided resource pointer, so make sure you don't delete it elsewhere.
     */
    explicit TResourceHandle(T *ptr, const FUuid &uuid)
            : TResourceHandleBase<WeakHandle>() {
        this->mData = q_new<FResourceHandleData>();
        this->addRef();

        this->setHandleData(ptr, uuid);
        this->bIsCreated = true;
    }

    /**
     * Constructs an invalid handle with the specified UUID. You must call setHandleData() with the actual resource
     * pointer to make the handle valid.
     */
    TResourceHandle(const FUuid &uuid) {
        this->mData = q_new<FResourceHandleData>();
        this->mData->uuid = uuid;

        this->addRef();
    }

    /**	Replaces the internal handle data pointer, effectively transforming the handle into a different handle. */
    void setHandleData(FResourceHandleData *data) {
        this->releaseRef();
        this->mData = data;
        this->addRef();
    }

    /**	Converts a weak handle into a normal handle. */
    TResourceHandle<T, false> lock() const {
        TResourceHandle<FResource, false> handle;
        handle.setHandleData(this->getHandleData());

        return handle;
    }

    using FResourceHandleBase::setHandleData;
};

/**	Checks if two handles point to the same resource. */
template<class _Ty1, bool _Weak1, class _Ty2, bool _Weak2>
bool operator==(const TResourceHandle<_Ty1, _Weak1> &_Left, const TResourceHandle<_Ty2, _Weak2> &_Right) {
    if (_Left.getHandleData() != nullptr && _Right.getHandleData() != nullptr)
        return _Left.getHandleData()->mPtr == _Right.getHandleData()->mPtr;

    return _Left.getHandleData() == _Right.getHandleData();
}

/**	Checks if a handle is null. */
template<class _Ty1, bool _Weak1, class _Ty2, bool _Weak2>
bool operator==(const TResourceHandle<_Ty1, _Weak1> &_Left, std::nullptr_t _Right) {
    return _Left.getHandleData() == nullptr || _Left.getHandleData()->mUUID.empty();
}

template<class _Ty1, bool _Weak1, class _Ty2, bool _Weak2>
bool operator!=(const TResourceHandle<_Ty1, _Weak1> &_Left, const TResourceHandle<_Ty2, _Weak2> &_Right) {
    return (!(_Left == _Right));
}

template <typename T>
using FResourceHandle = TResourceHandle<T, false>;

/**
 * @copydoc ResourceHandleBase
 *
 * Weak handles don't prevent the resource from being unloaded.
 */
template <typename T>
using FWeakResourceHandle = TResourceHandle<T, true>;

/**	Casts one resource handle to another. */
template<class _Ty1, class _Ty2, bool _Weak2, bool _Weak1>
TResourceHandle<_Ty1, _Weak1> StaticResourceCast(const TResourceHandle<_Ty2, _Weak2>& other) {
    TResourceHandle<_Ty1, _Weak1> handle;
    handle.setHandleData(other.getHandleData());

    return handle;
}

/**	Casts one resource handle to another. */
template<class _Ty1, class _Ty2, bool _Weak2>
TResourceHandle<_Ty1, false> StaticResourceCast(const TResourceHandle<_Ty2, _Weak2>& other) {
    TResourceHandle<_Ty1, false> handle;
    handle.setHandleData(other.getHandleData());

    return handle;
}