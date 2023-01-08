#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/String.h"

struct DLL_EXPORT FPath {
public:
    enum class PathType {
        Windows,
        Unix,
        Default,
    };

public:
    static FPath Empty;

private:
    TArray<FString> mDirectories;
    FString mNode;
    FString mDevice;
    FString mFilename;
    FString mExtension;

    bool mIsAbsolute = false;

public:
    FPath() = default;
    FPath(const FString &path, PathType type = PathType::Default);
    FPath(const TCHAR *path, PathType type = PathType::Default);
    FPath(const FPath &other);

    ~FPath() = default;

public:
    FPath operator+(const FPath &rhs) const;

    bool operator==(const FPath &rhs) const;

    bool operator!=(const FPath &rhs) const;

public:
    static FPath Combine(const FPath &left, const FPath &right);

    static bool ComparePathElem(const FString& left, const FString& right);

public:
    void assign(const FPath &path);
    void assign(const FString &path, PathType type = PathType::Default);

    FPath &append(const FPath &path);

    FString buildForWindow() const;
    FString buildForUnix() const;

    void clear();

    bool equals(const FPath &path) const;

    void parseWindow(const FString &path, PathType type = PathType::Default);
    void parseUnix(const FString &path, PathType type = PathType::Default);

    void pushDirectory(const FString &dir);

    FPath &makeAbsolute(const FPath &base);
    FPath &makeRelative(const FPath& base);

    bool includes(const FPath &child) const;

    void setNode(const FString &node);
    void setDevice(const FString &device);
    void setFilename(const FString &filename);
    void setExtension(const FString &extensions);

    FString toString(PathType type = PathType::Default) const;

    bool isDirectory() const { return mFilename.empty(); }
    bool isFile() const { return !mFilename.empty(); }
    bool isEmpty() const { return mDirectories.empty() && mFilename.empty() && mDevice.empty() && mNode.empty(); }
    bool isAbsolute() const { return mIsAbsolute; }

    bool isSubPathOf(const FPath &rhs);
    FPath getParent() const;
    FPath getDirectory() const;
    const FString &getDirectory(uint32_t i) const;

    FPath &makeParent();

    const FString &getNode() const { return mNode; }
    const FString &getDevice() const { return mDevice; }
    const FString &getFilename() const { return mFilename; }
    const FString &getExtension() const { return mExtension; }

    uint32_t getDirectoryCount() const { return static_cast<uint32_t>(mDirectories.length()); }
};

template<>
struct std::hash<FPath> {
    std::size_t operator()(const FPath &path) const {
        auto h = hash<FString>{};
        return h(path.toString());
    }
};