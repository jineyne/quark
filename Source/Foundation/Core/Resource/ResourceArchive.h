#pragma once

#include "CorePrerequisites.h"

#include "FileSystem/FileSystem.h"
#include "Misc/Uuid.h"

enum class EResourceArchiveType {
    Original,
    Override,
};

class DLL_EXPORT ResourceArchive {
protected:
    Path mBasePath;
    EResourceArchiveType mType;

    TMap<Uuid, Path> mUuidToPathMap;
    TMap<Path, Uuid> mPathToUuidMap;

public:
    ResourceArchive(const Path &path, EResourceArchiveType type);

public:
    virtual bool initialize() = 0;

public:
    bool exists(const Path &path);
    bool exists(const Uuid &uuid);

    virtual std::shared_ptr<Stream> load(const Path &path);
    virtual std::shared_ptr<Stream> load(const Uuid &uuid);

    Uuid getUuidFromPath(const Path &path);
    const Path &getPathFromUuid(const Uuid &uuid);

protected:
    virtual std::shared_ptr<Stream> loadInternal(const Path &path) = 0;
};

class DLL_EXPORT DirectoryResourceArchive : public ResourceArchive {
public:
    DirectoryResourceArchive(const Path &path, EResourceArchiveType type);

public:
    bool initialize() override;

protected:
    std::shared_ptr<Stream> loadInternal(const Path &path) override;

private:
    bool parseDirectory(const Path &dir);
};