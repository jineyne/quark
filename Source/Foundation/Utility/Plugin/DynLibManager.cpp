#include "DynLibManager.h"

#include "Misc/StringBuilder.h"

static bool operator<(const FDynLib *lhs, const FString &rhs) {
    return lhs->getName().compare(rhs) < 0;
}

static bool operator<(const FString &lhs, const FDynLib *rhs) {
    return rhs->getName().compare(lhs) < 0;
}

FDynLib *FDynLibManager::load(const FString &name) {
    size_t length = name.length();
    const FString extension = FString(TEXT(".")) + FDynLib::Extension;
    size_t extLength = extension.length();

    FStringBuilder ss(512);

    if (FDynLib::Prefix != nullptr) {
        ss << FDynLib::Prefix;
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
        auto lib = q_new<FDynLib>(realName);
        mLoadedLibraries.add(lib->getName(), lib);

        return lib;
    }
}

void FDynLibManager::unload(FDynLib *lib) {
    mLoadedLibraries.remove(lib->getName());

    q_delete(lib);
}

void FDynLibManager::onShutDown() {
    for (auto it : mLoadedLibraries) {
        q_delete(it.value);
    }

    mLoadedLibraries.reset();
}

FDynLibManager &gDynLibManager() {
    return FDynLibManager::Instance();
}
