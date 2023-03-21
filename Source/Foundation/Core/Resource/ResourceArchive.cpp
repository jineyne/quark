#include "ResourceArchive.h"
#include "Misc/Platform.h"

FResourceArchive::FResourceArchive(const FPath &path, EResourceArchiveType type) {

}

bool FResourceArchive::exists(const FPath &path) {
    auto it = mPathToUuidMap.find(path);

    if (it != nullptr) {
        return true;
    }

    return false;
}

bool FResourceArchive::exists(const FUuid &uuid) {
    auto it = mUuidToPathMap.find(uuid);

    if (it != nullptr) {
        return true;
    }

    return false;
}

std::shared_ptr<FStream> FResourceArchive::load(const FPath &path) {
    return loadInternal(path);
}

std::shared_ptr<FStream> FResourceArchive::load(const FUuid &uuid) {
    return loadInternal(getPathFromUuid(uuid));
}

FUuid FResourceArchive::getUuidFromPath(const FPath &path) {
    auto it = mPathToUuidMap.find(path);

    if (it != nullptr) {
        return *it;
    } else {
        return FUuid::Empty;
    }
}

const FPath &FResourceArchive::getPathFromUuid(const FUuid &uuid) {
    auto it = mUuidToPathMap.find(uuid);

    if (it != nullptr) {
        return *it;
    } else {
        return FPath::Empty;
    }
}

FDirectoryResourceArchive::FDirectoryResourceArchive(const FPath &path, EResourceArchiveType type)
        : FResourceArchive(path, type) {

}

bool FDirectoryResourceArchive::initialize() {
    if (!FFileSystem::IsDirectory(mBasePath)) {
        return false;
    }

    return parseDirectory(mBasePath);
}

std::shared_ptr<FStream> FDirectoryResourceArchive::loadInternal(const FPath &path) {
    return FFileSystem::OpenFile(path);
}

bool FDirectoryResourceArchive::parseDirectory(const FPath &dir) {
    TArray<FPath> directories, files;

    FFileSystem::GetChildren(dir, files, directories);
    for (auto &dir : directories) {
        if (!parseDirectory(dir)) {
            return false;
        }
    }

    for (auto &file : files) {
        // TODO: CHECK IF FILE IS MARU FORMAT THEN USE UUID FROM FILE
        auto uuid = FPlatform::GenerateUUID();
        mUuidToPathMap[uuid] = file;
        mPathToUuidMap[file] = uuid;
    }

    return true;
}
