#include "Win32.h"

#include "Logging/LogDefines.h"
#include "FileSystem/FileSystem.h"
#include "Misc/StringBuilder.h"

static void Win32HandleError(DWORD error, const Path &path) {
    String raw = path.toString();

    FStringBuilder ss(512);
    switch (error) {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
        case ERROR_BAD_NETPATH:
        case ERROR_CANT_RESOLVE_FILENAME:
        case ERROR_INVALID_DRIVE:
            LOG(LogFileSystem, Error, TEXT("Path \"%ls\" not found."), *raw);
            break;
        case ERROR_ACCESS_DENIED:
            LOG(LogFileSystem, Error, TEXT("Access to path \"%ls\" denied."), *raw);
            break;
        case ERROR_ALREADY_EXISTS:
        case ERROR_FILE_EXISTS:
            LOG(LogFileSystem, Error, TEXT("File/folder at path \"%ls\" already exists."), *raw);
            break;
        case ERROR_INVALID_NAME:
        case ERROR_DIRECTORY:
        case ERROR_FILENAME_EXCED_RANGE:
        case ERROR_BAD_PATHNAME:
            LOG(LogFileSystem, Error, TEXT("Invalid path string: \"%ls\"."), *raw);
            break;
        case ERROR_FILE_READ_ONLY:
            LOG(LogFileSystem, Error, TEXT("File at path \"%ls\" is read only."), *raw);
            break;
        case ERROR_CANNOT_MAKE:
            LOG(LogFileSystem, Error, TEXT("Cannot create file/folder at path: \"%ls\"."), *raw);
            break;
        case ERROR_DIR_NOT_EMPTY:
            LOG(LogFileSystem, Error, TEXT("Directory at path \"%ls\" not empty."), *raw);
            break;
        case ERROR_WRITE_FAULT:
            LOG(LogFileSystem, Error, TEXT("Error while writing a file at path \"%ls\"."), *raw);
            break;
        case ERROR_READ_FAULT:
            LOG(LogFileSystem, Error, TEXT("Error while reading a file at path \"%ls\"."), *raw);
            break;
        case ERROR_SHARING_VIOLATION:
            LOG(LogFileSystem, Error, TEXT("Sharing violation at path \"%ls\"."), *raw);
            break;
        case ERROR_LOCK_VIOLATION:
            LOG(LogFileSystem, Error, TEXT("Lock violation at path \"%ls\"."), *raw);
            break;
        case ERROR_HANDLE_EOF:
            LOG(LogFileSystem, Error, TEXT("End of file reached for file at path \"%ls\"."), *raw);
            break;
        case ERROR_HANDLE_DISK_FULL:
        case ERROR_DISK_FULL:
            LOG(LogFileSystem, Error, TEXT("Disk full."));
            break;
        case ERROR_NEGATIVE_SEEK:
            LOG(LogFileSystem, Error, TEXT("Negative seek."));
            break;
        default:
            LOG(LogFileSystem, Error, TEXT("Undefined file system exception: %ls"), (UINT32)error);
            assert(true);
            break;
    }
}

bool Win32Exists(const Path &path) {
    DWORD attr = GetFileAttributes(*path.toString());
    if (attr == 0xffffffff) {
        switch (GetLastError()) {
            case ERROR_FILE_NOT_FOUND:
            case ERROR_PATH_NOT_FOUND:
            case ERROR_NOT_READY:
            case ERROR_INVALID_DRIVE:
                return false;
            default:
                Win32HandleError(GetLastError(), path);
        }
    }
    return true;
}

bool Win32IsDirectory(const Path &path) {
    DWORD attr = GetFileAttributes(*path.toString());
    if (attr == 0xffffffff) {
        Win32HandleError(GetLastError(), path);
    }

    return (attr & FILE_ATTRIBUTE_DIRECTORY) != FALSE;
}

bool Win32CreateDirectory(const Path& path) {
    if (Win32Exists(path) && Win32IsDirectory(path)) {
        return false;
    }

    if (CreateDirectory(*path.toString(), 0) == FALSE) {
        Win32HandleError(GetLastError(), path);
    }

    return true;
}

bool FileSystem::GetChildren(const Path &path, TArray<Path> &files, TArray<Path> &directories) {
    auto findPath = path.toString();
    if (IsFile(path)) {
        return false;
    }

    if (path.isFile()) {
        findPath.append("\\*");
    } else {
        findPath.append("*");
    }

    WIN32_FIND_DATA data;
    HANDLE handle = FindFirstFile(*findPath, &data);
    if (handle == INVALID_HANDLE_VALUE) {
        Win32HandleError(GetLastError(), path);
        return false;
    }

    String name;
    do {
        name = data.cFileName;
        if (name != "." && name != "..") {
            Path fullPath = path;
            if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
                fullPath.append(name);
                fullPath.append(TEXT("/"));

                directories.add(fullPath);
            } else {
                files.add(fullPath.append(name));
            }
        }

        if(FindNextFile(handle, &data) == FALSE) {
            if (GetLastError() != ERROR_NO_MORE_FILES) {
                Win32HandleError(GetLastError(), findPath);
            }

            break;
        }
    } while (true);

    return true;
}

Path FileSystem::GetWorkingDirectoryPath() {
    DWORD len = GetCurrentDirectory(0, nullptr);
    if (len > 0) {
        auto *buf = q_alloc<TCHAR>(len);
        DWORD n = GetCurrentDirectory(len, buf);
        if (n > 0 && n <= len) {
            FStringBuilder ss(len * 2);
            ss.append(buf, n);
            if (buf[len - 1] != '\\') {
                ss.appendChar('\\');
            }

            q_delete(buf);
            return ss.toString();
        }

        q_delete(buf);
    }

    return Path(String::Empty);
}

void FileSystem::SetWorkingDirectoryPath(const Path &path) {
    auto raw = path.toString();
    if (!SetCurrentDirectory(*raw)) {
        LOG(LogFileSystem, Error, TEXT("Failed to set working directory path: %ls"), *raw);
    }
}

Path FileSystem::GetTempDirectoryPath() {
    DWORD len = GetTempPath(0, nullptr);
    if (len > 0) {
        auto *buf = q_alloc<TCHAR>(len);
        DWORD n = GetTempPath(len, buf);
        if (n > 0 && n <= len) {
            FStringBuilder ss(len * 2);
            ss.append(buf, n);
            if (buf[len - 1] != '\\') {
                ss.appendChar('\\');
            }

            q_delete(buf);
            return ss.toString();
        }

        q_delete(buf);
    }

    return Path(String::Empty);
}

bool FileSystem::Exists(const Path &path) {
    return Win32Exists(path);
}

bool FileSystem::CreateDir(const Path& path) {
    Path parentPath = path;
    while (!Exists(parentPath) && parentPath.getDirectoryCount() > 0) {
        parentPath = parentPath.getParent();
    }

    for (uint32_t i = parentPath.getDirectoryCount(); i < path.getDirectoryCount(); i++) {
        parentPath.append(path.getDirectory(i));
        Win32CreateDirectory(parentPath);
    }

    if (path.isFile()) {
        Win32CreateDirectory(path);
    }

    return true;
}

bool FileSystem::Delete(const Path &path) {
    return DeleteFile(*path.toString());
}

bool FileSystem::IsDirectory(const Path &path) {
    return Win32IsDirectory(path);
}