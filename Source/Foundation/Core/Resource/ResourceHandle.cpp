#include "ResourceHandle.h"
#include "Exception/Exception.h"
#include "Resources.h"

bool ResourceHandleBase::isLoaded(bool checkDependencies) const {
    bool isLoaded = (mData != nullptr && mData->isLoaded && mData->ptr != nullptr);
    return isLoaded;
}

void ResourceHandleBase::blockUntilLoaded(bool waitForDependencies) const {
    if (mData == nullptr) {
        return;
    }
}

void ResourceHandleBase::release() {
    gResources().release(this);
}

void ResourceHandleBase::destroy() {
    gResources().destroy(this);
}

void ResourceHandleBase::setHandleData(Resource *ptr, const Uuid &uuid) {
    mData->ptr = ptr;

    if(mData->ptr) {
        mData->uuid = uuid;
    }
}

void ResourceHandleBase::clearHandleData() {
    mData->ptr = nullptr;
    mData->isLoaded = false;
}

void ResourceHandleBase::addInternalRef() {
    mData->refCount.fetch_add(1, std::memory_order_relaxed);
}

void ResourceHandleBase::removeInternalRef() {
    mData->refCount.fetch_sub(1, std::memory_order_relaxed);
}

void ResourceHandleBase::notifyLoadComplete() {
    if (!mData->isLoaded)
    {
        mData->isLoaded = true;
    }
}

void ResourceHandleBase::throwIfNotLoaded() const {
#if DEBUG_MODE
    if (!isLoaded(false)) {
        EXCEPT(LogResource, InternalErrorException, TEXT("Trying to access a resource that hasn't been loaded yet."));
    }
#endif
}
