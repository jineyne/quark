#pragma once

#include "UtilityCore.h"
#include "Misc/Module.h"
#include "DynLib.h"
#include "DynLibManager.g.h"

QCLASS()
class DLL_EXPORT FDynLibManager : public TModule<FDynLibManager> {
public:
    GENERATED_BODY();

public:
    using DynLibMap = TMap<FString, FDynLib *>;

private:
    DynLibMap mLoadedLibraries;

public:
    FDynLib *load(const FString &name);
    void unload(FDynLib *lib);

protected:
    void onShutDown() override;
};

DLL_EXPORT FDynLibManager &gDynLibManager();