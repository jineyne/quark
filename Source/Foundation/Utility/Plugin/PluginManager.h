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
    QFUNCTION()
    static void RegisterFactory(IPluginFactory *factory);

private:
    QFUNCTION()
    static TArray<PluginHandle> &GetFactoryHandleList();

public:
    QFUNCTION()
    PluginId loadPlugin(const FString &name);

    QFUNCTION()
    void updatePlugin(const PluginId &id);

    QFUNCTION()
    void updateAll();

    QFUNCTION()
    void unloadPlugin(const PluginId &id);

protected:
    QFUNCTION()
    void onShutDown() override;
};

DLL_EXPORT FPluginManager &gPluginManager();
