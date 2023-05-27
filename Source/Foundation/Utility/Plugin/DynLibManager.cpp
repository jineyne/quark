#include "DynLibManager.h"

#include "Misc/StringBuilder.h"

static bool operator<(const DynLib *lhs, const String &rhs) {
    return lhs->getName().compare(rhs) < 0;
}

static bool operator<(const String &lhs, const DynLib *rhs) {
    return rhs->getName().compare(lhs) < 0;
}

DynLib *DynLibManager::load(const String &name) {
    size_t length = name.length();
    const String extension = String(TEXT(".")) + DynLib::Extension;
    size_t extLength = extension.length();

    FStringBuilder ss(512);

    if (DynLib::Prefix != nullptr) {
        ss << DynLib::Prefix;
    }

    ss << name;

    if (length <= extLength || name.right(length - extLength) != extension) {
        ss << extension;
    }

    auto realName = ss.toString();

    auto found = mLoadedLibraries.find(realName);
    if (found != nullptr && (*found)->getName() == realName) {
        return (*found);
    } else {
        auto lib = q_new<DynLib>(realName);
        mLoadedLibraries.add(lib->getName(), lib);

        return lib;
    }
}

void DynLibManager::unload(DynLib *lib) {
    mLoadedLibraries.remove(lib->getName());

    q_delete(lib);
}

void DynLibManager::onShutDown() {
    for (auto it : mLoadedLibraries) {
        q_delete(it.value);
    }

    mLoadedLibraries.reset();
}

DynLibManager &gDynLibManager() {
    return DynLibManager::Instance();
}
