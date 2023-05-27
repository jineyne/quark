#include "Resources.h"

DEFINE_LOG_CATEGORY(LogResource)

Resources::Resources() {

}

void Resources::update(HResource &handle, Resource *resource) {
    const Uuid& uuid = handle.getUUID();
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

void Resources::release(ResourceHandleBase *resource) {
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

void Resources::destroy(ResourceHandleBase *resource) {
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

HResource Resources::createResourceHandle(Resource *obj, bool builtin) {
    auto uuid = Platform::GenerateUUID();
    return createResourceHandle(obj, uuid, builtin);
}

HResource Resources::createResourceHandle(Resource *obj, const Uuid &uuid, bool builtin) {
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

Resources &gResources() {
    return Resources::Instance();
}
