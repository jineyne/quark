#include "FileSystem.h"

#include "Logging/LogDefines.h"

#include "FileStream.h"
#include "MemoryStream.h"

DEFINE_LOG_CATEGORY(LogFileSystem);

void FFileSystem::Initialize() {
}

void FFileSystem::Finalize() {
}

std::shared_ptr<FStream> FFileSystem::OpenFile(const FPath &path, bool readOnly) {
    if (!Exists(path)) {
        LOG(LogFileSystem, Error, TEXT("Unable to find file: %s"), *path.toString());
    }

    if (IsDirectory(path)) {
        LOG(LogFileSystem, Error, TEXT("Trying to open directory: %s"), *path.toString());
    }

    EStreamAccessMode access = (readOnly) ? EStreamAccessMode::Read : EStreamAccessMode::Write;
    return std::make_shared<FFileStream>(path, access);
}

TSharedPtr<FStream> FFileSystem::CreateAndOpenFile(const FPath &path) {
    return std::make_shared<FFileStream>(path, EStreamAccessMode::Write, true);
}

bool FFileSystem::IsFile(const FPath &path) {
    if (Exists(path)) {
        return !IsDirectory(path);
    }

    return false;
}