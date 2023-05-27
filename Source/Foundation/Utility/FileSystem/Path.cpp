#include "Path.h"

#include "Logging/LogDefines.h"
#include "Misc/StringBuilder.h"

Path Path::Empty;

Path::Path(const String &path, Path::PathType type) {
    assign(path, type);
}

Path::Path(const TCHAR *path, Path::PathType type) {
    assign(path, type);
}

Path::Path(const Path &other) {
    assign(other);
}

Path Path::operator+(const Path &rhs) const {
    return Path::Combine(*this, rhs);
}

bool Path::operator==(const Path &rhs) const {
    return equals(rhs);
}

bool Path::operator!=(const Path &rhs) const {
    return !equals(rhs);
}

Path Path::Combine(const Path &left, const Path &right) {
    Path output = left;
    return output.append(right);
}

bool Path::ComparePathElem(const String &left, const String &right) {
    return left.lower() == right.lower();
}

void Path::assign(const Path &path) {
    mDirectories = path.mDirectories;
    mFilename = path.mFilename;
    mExtension = path.mExtension;
    mDevice = path.mDevice;
    mNode = path.mNode;
    mIsAbsolute = path.mIsAbsolute;
}

void Path::assign(const String &path, Path::PathType type) {
#if PLATFORM == PLATFORM_WIN32
    parseWindow(path, type);
#else
    parseUnix(path, type);
#endif
}

Path &Path::append(const Path &path) {
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

String Path::buildForWindow() const {
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

    return String(ss.getData(), ss.length());
}

String Path::buildForUnix() const {
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

    return String(ss.getData(), ss.length());
}

void Path::clear() {
    mDirectories.clear();
    mFilename = "";
}

bool Path::equals(const Path &path) const {
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

void Path::parseWindow(const String &path, Path::PathType type) {
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
                if (mIsAbsolute || !CString::IsAlpha(device)) {
                    LOG(LogFileSystem, Error, TEXT("Invalid path: %s"), *path);
                }

                mIsAbsolute = true;
                setDevice(String(1, device));

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

void Path::parseUnix(const String &path, Path::PathType type) {
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
                    auto deviceStr = String(sb.toString());
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

void Path::pushDirectory(const String &dir) {
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

Path &Path::makeAbsolute(const Path &base) {
    if (mIsAbsolute) {
        return *this;
    }

    Path absDir = base.getDirectory();
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

Path & Path::makeRelative(const Path &base) {
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

bool Path::includes(const Path &child) const {
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

void Path::setNode(const String &node) {
    mNode = node;
}

void Path::setDevice(const String &device) {
    mDevice = device;
}

void Path::setFilename(const String &filename) {
    int32_t offset = filename.findLastChar('.');
    if (offset != INDEX_NONE) {
        mFilename = const_cast<String &>(filename).mid(0, offset);
        mExtension = const_cast<String &>(filename).mid(offset + 1, filename.length() - (offset + 1));
    } else {
        mFilename = filename;
    }
}

void Path::setExtension(const String &extension) {
    if (extension[0] == '.') {
        mExtension = const_cast<String &>(extension).mid(1, extension.length() - 1);
    } else {
        mExtension = extension;
    }
}

String Path::toString(Path::PathType type) const {
    switch (type) {
        case Path::PathType::Windows:
            return buildForWindow();

        case Path::PathType::Unix:
            return buildForUnix();
        default:
#if PLATFORM == PLATFORM_WIN32
            return buildForWindow();
#else
            return buildForUnix();
#endif
    }
}

bool Path::isSubPathOf(const Path &rhs) {
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

Path Path::getParent() const {
    Path copy = *this;
    copy.makeParent();

    return copy;
}

Path Path::getDirectory() const {
    Path copy = *this;
    copy.mFilename.clear();
    copy.mExtension.clear();

    return copy;
}

const String &Path::getDirectory(uint32_t i) const {
    return mDirectories[i];
}

Path &Path::makeParent() {
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