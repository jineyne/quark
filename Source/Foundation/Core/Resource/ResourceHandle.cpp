#include "ResourceHandle.h"
#include "Exception/Exception.h"
#include "Resources.h"

bool FResourceHandleBase::isLoaded(bool checkDependencies) const {
    bool isLoaded = (mData != nullptr && mData->isLoaded && mData->ptr != nullptr);
    return isLoaded;
}

void FResourceHandleBase::blockUntilLoaded(bool waitForDependencies) const {
    if (mData == nullptr) {
        return;
    }
}

void FResourceHandleBase::release() {
    gResources().release(this);
}

void FResourceHandleBase::destroy() {
    gResources().destroy(this);
}

void FResourceHandleBase::setHandleData(FResource *ptr, const FUuid &uuid) {
    mData->ptr = ptr;

    if(mData->ptr) {
        mData->uuid = uuid;
    }
}

void FResourceHandleBase::clearHandleData() {
    mData->ptr = nullptr;
    mData->isLoaded = false;
}

void FResourceHandleBase::addInternalRef() {
    mData->refCount.fetch_add(1, std::memory_order_relaxed);
}

void FResourceHandleBase::removeInternalRef() {
    mData->refCount.fetch_sub(1, std::memory_order_relaxed);
}

void FResourceHandleBase::notifyLoadComplete() {
    if (!mData->isLoaded)
    {
        mData->isLoaded = true;
    }
}

void FResourceHandleBase::throwIfNotLoaded() const {
#if DEBUG_MODE
    if (!isLoaded(false)) {
        EXCEPT(FLogResource, InternalErrorException, TEXT("Trying to access a resource that hasn't been loaded yet."));
    }
#endif
}
