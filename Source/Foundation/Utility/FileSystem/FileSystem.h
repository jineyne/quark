#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Container/Array.h"

#include "Path.h"
#include "Stream.h"

class DLL_EXPORT FFileSystem {
public:
    static void Initialize();
    static void Finalize();

    static std::shared_ptr<FStream> OpenFile(const FPath &path, bool readOnly = true);
    static std::shared_ptr<FStream> CreateAndOpenFile(const FPath &path);

    static bool Exists(const FPath &path);
    static bool CreateDir(const FPath &path);
    static bool Delete(const FPath &path);
    static bool IsFile(const FPath &path);
    static bool IsDirectory(const FPath &path);
    static bool GetChildren(const FPath &path, TArray<FPath> &files, TArray<FPath> &directories);

    static FPath GetWorkingDirectoryPath();
    static void SetWorkingDirectoryPath(const FPath &path);
    static FPath GetTempDirectoryPath();
};