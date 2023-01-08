#include "Path.h"

#include "Logging/LogDefines.h"
#include "Misc/StringBuilder.h"

FPath FPath::Empty;

FPath::FPath(const FString &path, FPath::PathType type) {
    assign(path, type);
}

FPath::FPath(const TCHAR *path, FPath::PathType type) {
    assign(path, type);
}

FPath::FPath(const FPath &other) {
    assign(other);
}

FPath FPath::operator+(const FPath &rhs) const {
    return FPath::Combine(*this, rhs);
}

bool FPath::operator==(const FPath &rhs) const {
    return equals(rhs);
}

bool FPath::operator!=(const FPath &rhs) const {
    return !equals(rhs);
}

FPath FPath::Combine(const FPath &left, const FPath &right) {
    FPath output = left;
    return output.append(right);
}

bool FPath::ComparePathElem(const FString &left, const FString &right) {
    return left.lower() == right.lower();
}

void FPath::assign(const FPath &path) {
    mDirectories = path.mDirectories;
    mFilename = path.mFilename;
    mExtension = path.mExtension;
    mDevice = path.mDevice;
    mNode = path.mNode;
    mIsAbsolute = path.mIsAbsolute;
}

void FPath::assign(const FString &path, FPath::PathType type) {
#if PLATFORM == PLATFORM_WIN32
    parseWindow(path, type);
#else
    parseUnix(path, type);
#endif
}

FPath &FPath::append(const FPath &path) {
    if (!mFilename.empty()) {
        pushDirectory(mFilename);
    }

    for (auto &dir : path.mDirectories) {
        pushDirectory(dir);
    }

    mFilename = path.mFilename;
    mExtension = path.mExtension;

    return *this;
}

FString FPath::buildForWindow() const {
    FStringBuilder ss(512);

    if (!mNode.empty()) {
        ss << TEXT("//") << mNode << TEXT("/");
    } else if (!mDevice.empty()) {
        ss << mDevice << ":/";
    } else if (mIsAbsolute) {
        ss << "./";
    }

    for (auto &dir : mDirectories) {
        ss << dir << "/";
    }

    ss << mFilename;
    if (!mExtension.empty()) {
        ss << "." << mExtension;
    }

    return FString(ss.getData(), ss.length());
}

FString FPath::buildForUnix() const {
    FStringBuilder ss(512);

    auto iter = mDirectories.begin();

    if (!mDevice.empty()) {
        ss << TEXT("/") << mDevice << TEXT(":/");
    } else if (mIsAbsolute) {
        if (iter != mDirectories.end() && *iter == TEXT("~")) {
            ss << TEXT("~");
            iter++;
        }

        ss << TEXT("/");
    }

    for (; iter != mDirectories.end(); iter++) {
        ss << *iter << TEXT("/");
    }

    ss << mFilename;
    if (!mExtension.empty()) {
        ss << TEXT(".") << mExtension;
    }

    return FString(ss.getData(), ss.length());
}

void FPath::clear() {
    mDirectories.clear();
    mFilename = "";
}

bool FPath::equals(const FPath &path) const {
    if (mIsAbsolute != path.mIsAbsolute) {
        return false;
    }

    if (mIsAbsolute) {
        if (mDevice != path.mDevice) {
            return false;
        }
    }

    if (mNode != path.mNode) {
        return false;
    }

    uint32_t count = mDirectories.length();
    uint32_t otherCount = path.mDirectories.length();

    if (!mFilename.empty()) {
        count++;
    }

    if (!path.mFilename.empty()) {
        otherCount++;
    }

    if (count != otherCount) {
        return false;
    }

    if (count > 0) {
        auto itMe = mDirectories.begin();
        auto itOther = path.mDirectories.begin();

        for (uint32_t i = 0; i < (count - 1); i++, itMe++, itOther++) {
            if (itMe->lower() != itOther->lower()) {
                return false;
            }
        }

        if (!mFilename.empty()) {
            if (!path.mFilename.empty()) {
                if (mFilename.lower() != path.mFilename.lower()) {
                    return false;
                }
            } else {
                if (mFilename.lower() != itOther->lower()) {
                    return false;
                }
            }
        } else {
            if (!path.mFilename.empty()) {
                if (itMe->lower() != path.mFilename.lower()) {
                    return false;
                }
            } else {
                if (itMe->lower() != itOther->lower()) {
                    return false;
                }
            }
        }
    }

    return true;
}

void FPath::parseWindow(const FString &path, FPath::PathType type) {
    const uint32_t len = (uint32_t)path.length();
    uint32_t idx = 0;
    FStringBuilder sb(512);

    if (idx < len) {
        if (path[idx] == '\\' || path[idx] == '/') {
            mIsAbsolute = true;
            idx++;
        }
    }

    if (idx < len) {
        if (mIsAbsolute && (path[idx] == '\\' || path[idx] == '/')) {
            idx++;
            sb.reset();
            while (idx < len && path[idx] != '\\' && path[idx] != '/') {
                sb << path[idx++];
            }

            setNode(sb.toString());

            if (idx < len) {
                idx++;
            }
        } else {
            TCHAR device = path[idx++];

            if (idx < len && path[idx] == ':') {
                if (mIsAbsolute || !FCString::IsAlpha(device)) {
                    LOG(LogFileSystem, Error, TEXT("Invalid path: %s"), *path);
                }

                mIsAbsolute = true;
                setDevice(FString(1, device));

                idx++;

                if (idx >= len || (path[idx] != '\\' && path[idx] != '/')) {
                    LOG(LogFileSystem, Error, TEXT("Invalid path: %s"), *path);
                }

                idx++;
            } else {
                idx--;
            }
        }
    }

    while (idx < len) {
        sb.reset();
        while (idx < len && path[idx] != '\\' && path[idx] != '/') {
            sb << path[idx];
            idx++;
        }

        if (idx < len) {
            pushDirectory(sb.toString());
        } else {
            setFilename(sb.toString());
        }

        idx++;
    }
}

void FPath::parseUnix(const FString &path, FPath::PathType type) {
    const uint32_t len = (uint32_t)path.length();
    uint32_t idx = 0;
    FStringBuilder sb(512);

    if (idx < len) {
        if (path[idx] == '/') {
            mIsAbsolute = true;
            idx++;
        } else if (path[idx] == '~') {
            idx++;
            if (idx >= len || path[idx] == '/') {
                pushDirectory(TEXT("~"));
                mIsAbsolute = true;
                idx++;
            } else {
                idx--;
            }
        }

        while (idx < len) {
            sb.reset();
            while (idx < len && path[idx] != '/') {
                sb << path[idx];
                idx++;
            }

            if (idx < len) {
                if (mDirectories.empty()) {
                    auto deviceStr = FString(sb.toString());
                    if (!deviceStr.empty() && *(deviceStr.rbegin()) == ':') {
                        setDevice(deviceStr.mid(0, deviceStr.length() - 1));
                        mIsAbsolute = true;
                    } else {
                        pushDirectory(deviceStr);
                    }
                } else {
                    pushDirectory(sb.toString());
                }
            } else {
                setFilename(sb.toString());
            }

            idx++;
        }
    }
}

void FPath::pushDirectory(const FString &dir) {
    if (!dir.empty() && dir != TEXT(".")) {
        if (dir == TEXT("..")) {
            if (mDirectories.empty() && mDirectories.top() != TEXT("..")) {
                mDirectories.pop();
            } else {
                mDirectories.add(dir);
            }
        } else {
            mDirectories.add(dir);
        }
    }
}

FPath &FPath::makeAbsolute(const FPath &base) {
    if (mIsAbsolute) {
        return *this;
    }

    FPath absDir = base.getDirectory();
    if (base.isFile()) {
        absDir.pushDirectory(base.mFilename);
    }

    for (auto &dir : mDirectories) {
        absDir.pushDirectory(dir);
    }

    absDir.setFilename(mFilename);
    *this = absDir;

    return *this;
}

FPath & FPath::makeRelative(const FPath &base) {
    if (!base.includes(*this))
        return *this;

    mDirectories.removeRange(0, base.mDirectories.length());

    // Sometimes a directory name can be interpreted as a file and we're okay with that. Check for that
    // special case.
    if (base.isFile()) {
        if (mDirectories.empty()) {
            mFilename = "";
        } else {
            mDirectories.removeAt(0);
        }
    }

    mDevice = "";
    mNode = "";
    mIsAbsolute = false;

    return *this;
}

bool FPath::includes(const FPath &child) const {
    if (mDevice != child.mDevice) {
        return false;
    }

    if (mNode != child.mNode) {
        return false;
    }

    auto iterParent = mDirectories.begin();
    auto iterChild = child.mDirectories.begin();

    for (; iterParent != mDirectories.end(); ++iterChild, ++iterParent) {
        if (iterChild == child.mDirectories.end()) {
            return false;
        }

        if (!ComparePathElem(*iterChild, *iterParent)) {
            return false;
        }
    }

    if (!mFilename.empty()) {
        if (iterChild == child.mDirectories.end()) {
            if (child.mFilename.empty()) {
                return false;
            }

            if (!ComparePathElem(child.mFilename, mFilename)) {
                return false;
            }
        } else {
            if (!ComparePathElem(*iterChild, mFilename)) {
                return false;
            }
        }
    }

    return true;
}

void FPath::setNode(const FString &node) {
    mNode = node;
}

void FPath::setDevice(const FString &device) {
    mDevice = device;
}

void FPath::setFilename(const FString &filename) {
    int32_t offset = filename.findLastChar('.');
    if (offset != INDEX_NONE) {
        mFilename = const_cast<FString &>(filename).mid(0, offset);
        mExtension = const_cast<FString &>(filename).mid(offset + 1, filename.length() - (offset + 1));
    } else {
        mFilename = filename;
    }
}

void FPath::setExtension(const FString &extension) {
    if (extension[0] == '.') {
        mExtension = const_cast<FString &>(extension).mid(1, extension.length() - 1);
    } else {
        mExtension = extension;
    }
}

FString FPath::toString(FPath::PathType type) const {
    switch (type) {
        case FPath::PathType::Windows:
            return buildForWindow();

        case FPath::PathType::Unix:
            return buildForUnix();
        default:
#if PLATFORM == PLATFORM_WIN32
            return buildForWindow();
#else
            return buildForUnix();
#endif
    }
}

bool FPath::isSubPathOf(const FPath &rhs) {
    if (!isAbsolute()) {
        return true;
    }

    if (*this == rhs) {
        return false;
    }

    if (mDevice != rhs.mDevice) {
        return false;
    }

    if (mDirectories.length() < rhs.mDirectories.length()) {
        return false;
    }

    for (auto i = 0; i < rhs.mDirectories.length(); i++) {
        if (mDirectories[i] != rhs.mDirectories[i]) {
            return false;
        }
    }

    return true;
}

FPath FPath::getParent() const {
    FPath copy = *this;
    copy.makeParent();

    return copy;
}

FPath FPath::getDirectory() const {
    FPath copy = *this;
    copy.mFilename.clear();
    copy.mExtension.clear();

    return copy;
}

const FString &FPath::getDirectory(uint32_t i) const {
    return mDirectories[i];
}

FPath &FPath::makeParent() {
    if (mFilename.empty()) {
        if (mDirectories.empty()) {
            if (!mIsAbsolute) {
                mDirectories.add("..");
            }
        } else {
            if (mDirectories.top() == "..") {
                mDirectories.add("..");
            } else {
                mDirectories.pop();
            }
        }
    } else {
        mFilename.clear();
    }

    return *this;
}