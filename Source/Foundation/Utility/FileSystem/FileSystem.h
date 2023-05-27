#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Container/Array.h"

#include "Path.h"
#include "Stream.h"

class DLL_EXPORT FileSystem {
public:
    static void Initialize();
    static void Finalize();

    static std::shared_ptr<Stream> OpenFile(const Path &path, bool readOnly = true);
    static std::shared_ptr<Stream> CreateAndOpenFile(const Path &path);

    static bool Exists(const Path &path);
    static bool CreateDir(const Path &path);
    static bool Delete(const Path &path);
    static bool IsFile(const Path &path);
    static bool IsDirectory(const Path &path);
    static bool GetChildren(const Path &path, TArray<Path> &files, TArray<Path> &directories);

    static Path GetWorkingDirectoryPath();
    static void SetWorkingDirectoryPath(const Path &path);
    static Path GetTempDirectoryPath();
};