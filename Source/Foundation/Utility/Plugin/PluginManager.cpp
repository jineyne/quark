#include "PluginManager.h"

#include "DynLibManager.h"

DEFINE_LOG_CATEGORY(LogPlugin)

void PluginManager::RegisterFactory(IPluginFactory *factory) {
    assert(factory != nullptr);

    PluginHandle handle{};
    handle.loaded = false;
    handle.factory = factory;
    handle.lib = nullptr;

    GetFactoryHandleList().add(handle);
}

TArray<PluginHandle> &PluginManager::GetFactoryHandleList() {
    static TArray<PluginHandle> inst;
    return inst;
}

PluginId PluginManager::loadPlugin(const String &name) {
    auto list = GetFactoryHandleList();

    const PluginId count = static_cast<PluginId>(list.length());
    for (PluginId i = 0; i < count; i++) {
        auto &handle = list[i];
        if (handle.factory->name() == name) {
            handle.factory->loadPlugin();

            return i;
        }
    }

#if PLATFORM == PLATFORM_WIN32 || PLATFORM == PLATFORM_LINUX
    auto id = static_cast<PluginId>(list.length());
    auto lib = gDynLibManager().load(name);
    if (lib) {
        list = GetFactoryHandleList();
        auto handle = list.top();
        if (handle.factory->name() == name) {
            handle.factory->loadPlugin();
            handle.lib = lib;

            return id;
        }
    }
#endif

    EXCEPT(LogPlugin, InvalidParametersException, TEXT("Unable to find '%s'"), *name);
}

void PluginManager::updatePlugin(const PluginId &id) {
    auto list = GetFactoryHandleList();
    assert(id < list.length());

    list[id].factory->updatePlugin();
}

void PluginManager::updateAll() {
    auto list = GetFactoryHandleList();
    for (auto &handle : list) {
        handle.factory->updatePlugin();
    }
}

void PluginManager::unloadPlugin(const PluginId &id) {
    auto list = GetFactoryHandleList();
    assert(id < list.length());

    auto &handle = list[id];
    if (!handle.loaded) {
        return;
    }

    handle.factory->unloadPlugin();
    q_delete(handle.factory);

    if (handle.lib != nullptr) {
        gDynLibManager().unload(handle.lib);
    }

    handle.loaded = false;
}

void PluginManager::onShutDown() {
    auto list = GetFactoryHandleList();
    const auto count = static_cast<uint32_t>(list.length());
    for (uint32_t i = 0; i < count; i++) {
        auto &handle = list[i];
        if (handle.loaded) {
            continue;
        }

        handle.factory->unloadPlugin();
        q_delete(handle.factory);

        if (handle.lib != nullptr) {
            gDynLibManager().unload(handle.lib);
        }
        handle.loaded = false;
    }
}

PluginManager &gPluginManager() {
    return PluginManager::Instance();
}

