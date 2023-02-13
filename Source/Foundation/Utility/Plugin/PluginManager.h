#pragma once

#include "UtilityCore.h"
#include "DynLib.h"
#include "Misc/Module.h"
#include "PluginFactory.h"
#include "PluginManager.g.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPlugin, Debug)

#define PluginId uint64_t
#define InvalidPluginId static_cast<PluginId>(-1)

struct PluginHandle {
    bool loaded = false;
    FDynLib *lib;
    IPluginFactory *factory;
};

QCLASS()
class DLL_EXPORT FPluginManager : public TModule<FPluginManager> {
public:
    GENERATED_BODY();

public:
    static void RegisterFactory(IPluginFactory *factory);

private:
    static TArray<PluginHandle> &GetFactoryHandleList();

public:
    PluginId loadPlugin(const FString &name);

    void updatePlugin(const PluginId &id);
    void updateAll();
    void unloadPlugin(const PluginId &id);

protected:
    void onShutDown() override;
};

DLL_EXPORT FPluginManager &gPluginManager();
