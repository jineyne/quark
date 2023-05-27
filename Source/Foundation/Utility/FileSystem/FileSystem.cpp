#include "FileSystem.h"

#include "Logging/LogDefines.h"

#include "FileStream.h"
#include "MemoryStream.h"

DEFINE_LOG_CATEGORY(LogFileSystem);

void FileSystem::Initialize() {
}

void FileSystem::Finalize() {
}

std::shared_ptr<Stream> FileSystem::OpenFile(const Path &path, bool readOnly) {
    if (!Exists(path)) {
        LOG(LogFileSystem, Error, TEXT("Unable to find file: %ls"), *path.toString());
    }

    if (IsDirectory(path)) {
        LOG(LogFileSystem, Error, TEXT("Trying to open directory: %ls"), *path.toString());
    }

    EStreamAccessMode access = (readOnly) ? EStreamAccessMode::Read : EStreamAccessMode::Write;
    return std::make_shared<FileStream>(path, access);
}

TSharedPtr<Stream> FileSystem::CreateAndOpenFile(const Path &path) {
    return std::make_shared<FileStream>(path, EStreamAccessMode::Write, true);
}

bool FileSystem::IsFile(const Path &path) {
    if (Exists(path)) {
        return !IsDirectory(path);
    }

    return false;
}