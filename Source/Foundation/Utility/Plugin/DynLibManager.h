#pragma once

#include "UtilityCore.h"
#include "Misc/Module.h"
#include "DynLib.h"
#include "DynLibManager.g.h"

QCLASS()
class DLL_EXPORT DynLibManager : public TModule<DynLibManager> {
public:
    GENERATED_BODY();

public:
    using DynLibMap = TMap<String, DynLib *>;

private:
    DynLibMap mLoadedLibraries;

public:
    DynLib *load(const String &name);
    void unload(DynLib *lib);

protected:
    void onShutDown() override;
};

DLL_EXPORT DynLibManager &gDynLibManager();