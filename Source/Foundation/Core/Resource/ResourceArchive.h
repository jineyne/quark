#pragma once

#include "CorePrerequisites.h"

#include "FileSystem/FileSystem.h"
#include "Misc/Uuid.h"

enum class EResourceArchiveType {
    Original,
    Override,
};

class DLL_EXPORT FResourceArchive {
protected:
    FPath mBasePath;
    EResourceArchiveType mType;

    TMap<FUuid, FPath> mUuidToPathMap;
    TMap<FPath, FUuid> mPathToUuidMap;

public:
    FResourceArchive(const FPath &path, EResourceArchiveType type);

public:
    virtual bool initialize() = 0;

public:
    bool exists(const FPath &path);
    bool exists(const FUuid &uuid);

    virtual std::shared_ptr<FStream> load(const FPath &path);
    virtual std::shared_ptr<FStream> load(const FUuid &uuid);

    FUuid getUuidFromPath(const FPath &path);
    const FPath &getPathFromUuid(const FUuid &uuid);

protected:
    virtual std::shared_ptr<FStream> loadInternal(const FPath &path) = 0;
};

class DLL_EXPORT FDirectoryResourceArchive : public FResourceArchive {
public:
    FDirectoryResourceArchive(const FPath &path, EResourceArchiveType type);

public:
    bool initialize() override;

protected:
    std::shared_ptr<FStream> loadInternal(const FPath &path) override;

private:
    bool parseDirectory(const FPath &dir);
};