#pragma once

#include "CorePrerequisites.h"
#include "FileSystem/Path.h"
#include "ResourceArchive.h"
#include "ResourceHandle.h"
#include "Misc/Module.h"
#include "Resource.h"
#include "Resources.g.h"

DECLARE_LOG_CATEGORY_EXTERN(FLogResource, Debug)

QCLASS()
class DLL_EXPORT FResources : public TModule<FResources> {
    GENERATED_BODY()

private:
    struct LoadedResourceData;
    struct LoadedResourceData {
        LoadedResourceData() = default;
        LoadedResourceData(const FWeakResourceHandle<FResource>& resource, uint32_t size)
                :resource(resource), size(size)
        { }

        FWeakResourceHandle<FResource> resource;
        uint32_t internalRefCount = 0;
        uint32_t size = 0;
    };

    TMap<FUuid, FWeakResourceHandle<FResource>> mHandleMap;
    TMap<FUuid, LoadedResourceData> mLoadedResourceMap;
    TMap<FPath, FWeakResourceHandle<FResource>> mUnHandleMap;

    TArray<FResourceArchive *> mArchiveList;

public:
    FResources();

public:
    void update(HResource& handle, FResource *resource);

    void release(FResourceHandleBase *resource);
    void destroy(FResourceHandleBase *resource);

    HResource createResourceHandle(FResource *obj, bool builtin = false);
    HResource createResourceHandle(FResource *obj, const FUuid &uuid, bool builtin = false);

    void addResourcePath(const FPath &path, bool recurse = true);
};

DLL_EXPORT FResources &gResources();