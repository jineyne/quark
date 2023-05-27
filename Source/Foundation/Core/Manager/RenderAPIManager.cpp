#include "RenderAPIManager.h"

#include "RenderAPI/RenderAPI.h"
#include "Plugin/DynLibManager.h"

RenderAPIManager::~RenderAPIManager() {
    if (mRenderAPIInitialized) {
        RenderAPI::ShutDown();
    }

    auto factories = GetAvailableFactoryList();
    for (auto factory : factories) {
        q_delete(factory);
    }
}

void RenderAPIManager::RegisterFactory(RenderAPIFactory *factory) {
    assert(factory != nullptr);
    GetAvailableFactoryList().add(factory);
}

TArray<RenderAPIFactory *> &RenderAPIManager::GetAvailableFactoryList() {
    static TArray<RenderAPIFactory *> inst;
    return inst;
}

RenderWindow *RenderAPIManager::initialize(const String &pluginName, const RenderWindowDesc &desc) {
    if (mRenderAPIInitialized) {
        return nullptr;
    }

#if PLATFORM != PLATFORM_ANDROID
    auto *lib = gDynLibManager().load(pluginName);
    String name;

    if (lib != nullptr) {
        typedef const TCHAR *(*GetPluginNameFunc)();
        auto func = (GetPluginNameFunc) lib->getSymbol("getPluginName");
        if (func != nullptr) {
            name = func();
        }
    }
#else
    String name = pluginName;
#endif

    auto factories = GetAvailableFactoryList();
    for (auto factory : factories) {
        if (name == factory->name()) {
            factory->create();
            mRenderAPIInitialized = true;

            return gRenderAPI().initialize(desc);
        }
    }

    return nullptr;
}

RenderAPIManager &gRenderAPIManager() {
    return RenderAPIManager::Instance();
}
