#include "Resources.h"

DEFINE_LOG_CATEGORY(FLogResource)

FResources::FResources() {

}

void FResources::update(HResource &handle, FResource *resource) {
    const FUuid& uuid = handle.getUUID();
    handle.setHandleData(resource, uuid);
    handle.notifyLoadComplete();

    if(resource) {
        auto it = mLoadedResourceMap.find(uuid);
        if (it == nullptr) {
            LoadedResourceData& resData = mLoadedResourceMap[uuid];
            resData.resource = handle.getWeak();
        }
    }

    // onResourceModified(handle);

    // This method is not thread safe due to this call (callable from main thread only)
    // ResourceListenerManager::instance().notifyListeners(uuid);
}

void FResources::release(FResourceHandleBase *resource) {
    const auto &uuid = resource->getUUID();

    bool lostLastRef = false;
    {
        auto it = mLoadedResourceMap.find(uuid);
        if (it != nullptr) {
            LoadedResourceData &resData = *it;

            assert(resData.internalRefCount > 0);
            resData.internalRefCount--;

            std::uint32_t refCount = resource->getHandleData()->refCount.load(std::memory_order_relaxed);
            lostLastRef = refCount == 0;
        }
    }

    if (lostLastRef) {
        destroy(resource);
    }
}

void FResources::destroy(FResourceHandleBase *resource) {
    const auto &uuid = resource->getUUID();

    resource->mData->ptr->destroy();

    {
        auto it = mLoadedResourceMap.find(uuid);
        if (it != nullptr) {
            LoadedResourceData &data = *it;
            while (data.internalRefCount > 0) {
                data.internalRefCount--;
            }

            mLoadedResourceMap.remove(uuid);
        } else {
            assert(false); // This should never happen but in case it does fail silently in release mode
        }
    }

    if (!resource->bIsBuiltin) {
        q_delete(resource->mData);
    }
}

HResource FResources::createResourceHandle(FResource *obj, bool builtin) {
    auto uuid = FPlatform::GenerateUUID();
    return createResourceHandle(obj, uuid, builtin);
}

HResource FResources::createResourceHandle(FResource *obj, const FUuid &uuid, bool builtin) {
    HResource handle(obj, uuid);

    if (obj != nullptr) {
        LoadedResourceData &data = mLoadedResourceMap[uuid];
        data.resource = handle.getWeak();
    }
    mHandleMap[uuid] = handle.getWeak();

    if (builtin) {
        handle.setIsBuiltin();
    }

    handle.notifyLoadComplete();

    return handle;
}

FResources &gResources() {
    return FResources::Instance();
}
