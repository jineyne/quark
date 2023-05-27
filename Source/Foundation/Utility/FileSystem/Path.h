#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/String.h"

struct DLL_EXPORT Path {
public:
    enum class PathType {
        Windows,
        Unix,
        Default,
    };

public:
    static Path Empty;

private:
    TArray<String> mDirectories;
    String mNode;
    String mDevice;
    String mFilename;
    String mExtension;

    bool mIsAbsolute = false;

public:
    Path() = default;
    Path(const String &path, PathType type = PathType::Default);
    Path(const TCHAR *path, PathType type = PathType::Default);
    Path(const Path &other);

    ~Path() = default;

public:
    Path operator+(const Path &rhs) const;

    bool operator==(const Path &rhs) const;

    bool operator!=(const Path &rhs) const;

public:
    static Path Combine(const Path &left, const Path &right);

    static bool ComparePathElem(const String& left, const String& right);

public:
    void assign(const Path &path);
    void assign(const String &path, PathType type = PathType::Default);

    Path &append(const Path &path);

    String buildForWindow() const;
    String buildForUnix() const;

    void clear();

    bool equals(const Path &path) const;

    void parseWindow(const String &path, PathType type = PathType::Default);
    void parseUnix(const String &path, PathType type = PathType::Default);

    void pushDirectory(const String &dir);

    Path &makeAbsolute(const Path &base);
    Path &makeRelative(const Path& base);

    bool includes(const Path &child) const;

    void setNode(const String &node);
    void setDevice(const String &device);
    void setFilename(const String &filename);
    void setExtension(const String &extensions);

    String toString(PathType type = PathType::Default) const;

    bool isDirectory() const { return mFilename.empty(); }
    bool isFile() const { return !mFilename.empty(); }
    bool isEmpty() const { return mDirectories.empty() && mFilename.empty() && mDevice.empty() && mNode.empty(); }
    bool isAbsolute() const { return mIsAbsolute; }

    bool isSubPathOf(const Path &rhs);
    Path getParent() const;
    Path getDirectory() const;
    const String &getDirectory(uint32_t i) const;

    Path &makeParent();

    const String &getNode() const { return mNode; }
    const String &getDevice() const { return mDevice; }
    const String &getFilename() const { return mFilename; }
    const String &getExtension() const { return mExtension; }

    uint32_t getDirectoryCount() const { return static_cast<uint32_t>(mDirectories.length()); }
};

template<>
struct std::hash<Path> {
    std::size_t operator()(const Path &path) const {
        auto h = hash<String>{};
        return h(path.toString());
    }
};