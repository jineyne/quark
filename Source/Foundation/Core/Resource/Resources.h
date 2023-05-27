#pragma once

#include "CorePrerequisites.h"
#include "FileSystem/Path.h"
#include "ResourceArchive.h"
#include "ResourceHandle.h"
#include "Misc/Module.h"
#include "Resource.h"
#include "Resources.g.h"

DECLARE_LOG_CATEGORY_EXTERN(LogResource, Debug)

enum class EResourceLoadFlags {
    None = 0,
    LoadDependencies = 1 << 0,
    KeepInternalRef = 1 << 1,
    KeepSourceData = 1 << 2,
    Default = LoadDependencies | KeepInternalRef
};

ENUM_CLASS_FLAGS(EResourceLoadFlags)

QCLASS()
class DLL_EXPORT Resources : public TModule<Resources> {
    GENERATED_BODY()

private:
    struct LoadedResourceData;
    struct LoadedResourceData {
        LoadedResourceData() = default;
        LoadedResourceData(const FWeakResourceHandle<Resource>& resource, uint32_t size)
                :resource(resource), size(size)
        { }

        FWeakResourceHandle<Resource> resource;
        uint32_t internalRefCount = 0;
        uint32_t size = 0;
    };

    TMap<Uuid, FWeakResourceHandle<Resource>> mHandleMap;
    TMap<Uuid, LoadedResourceData> mLoadedResourceMap;
    TMap<Path, FWeakResourceHandle<Resource>> mUnHandleMap;

    TArray<ResourceArchive *> mArchiveList;

public:
    Resources();

public:
    FResourceHandle<Resource> load(const Path& filePath, EResourceLoadFlags loadFlags = EResourceLoadFlags::Default);
    void update(HResource& handle, Resource *resource);

    void release(ResourceHandleBase *resource);
    void destroy(ResourceHandleBase *resource);

    HResource createResourceHandle(Resource *obj, bool builtin = false);
    HResource createResourceHandle(Resource *obj, const Uuid &uuid, bool builtin = false);
};

DLL_EXPORT Resources &gResources();