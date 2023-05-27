#include "ResourceArchive.h"
#include "Misc/Platform.h"

ResourceArchive::ResourceArchive(const Path &path, EResourceArchiveType type) {

}

bool ResourceArchive::exists(const Path &path) {
    auto it = mPathToUuidMap.find(path);

    if (it != nullptr) {
        return true;
    }

    return false;
}

bool ResourceArchive::exists(const Uuid &uuid) {
    auto it = mUuidToPathMap.find(uuid);

    if (it != nullptr) {
        return true;
    }

    return false;
}

std::shared_ptr<Stream> ResourceArchive::load(const Path &path) {
    return loadInternal(path);
}

std::shared_ptr<Stream> ResourceArchive::load(const Uuid &uuid) {
    return loadInternal(getPathFromUuid(uuid));
}

Uuid ResourceArchive::getUuidFromPath(const Path &path) {
    auto it = mPathToUuidMap.find(path);

    if (it != nullptr) {
        return *it;
    } else {
        return Uuid::Empty;
    }
}

const Path &ResourceArchive::getPathFromUuid(const Uuid &uuid) {
    auto it = mUuidToPathMap.find(uuid);

    if (it != nullptr) {
        return *it;
    } else {
        return Path::Empty;
    }
}

DirectoryResourceArchive::DirectoryResourceArchive(const Path &path, EResourceArchiveType type)
        : ResourceArchive(path, type) {

}

bool DirectoryResourceArchive::initialize() {
    if (!FileSystem::IsDirectory(mBasePath)) {
        return false;
    }

    return parseDirectory(mBasePath);
}

std::shared_ptr<Stream> DirectoryResourceArchive::loadInternal(const Path &path) {
    return FileSystem::OpenFile(path);
}

bool DirectoryResourceArchive::parseDirectory(const Path &dir) {
    TArray<Path> directories, files;

    FileSystem::GetChildren(dir, files, directories);
    for (auto &dir : directories) {
        if (!parseDirectory(dir)) {
            return false;
        }
    }

    for (auto &file : files) {
        // TODO: CHECK IF FILE IS MARU FORMAT THEN USE Uuid FROM FILE
        auto uuid = Platform::GenerateUUID();
        mUuidToPathMap[uuid] = file;
        mPathToUuidMap[file] = uuid;
    }

    return true;
}
