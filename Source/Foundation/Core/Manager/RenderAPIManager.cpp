#include "RenderAPIManager.h"

#include "RenderAPI/RenderAPI.h"
#include "Plugin/DynLibManager.h"

FRenderAPIManager::~FRenderAPIManager() {
    if (mRenderAPIInitialized) {
        FRenderAPI::ShutDown();
    }

    auto factories = GetAvailableFactoryList();
    for (auto factory : factories) {
        delete factory;
    }
}

void FRenderAPIManager::RegisterFactory(FRenderAPIFactory *factory) {
    assert(factory != nullptr);
    GetAvailableFactoryList().add(factory);
}

TArray<FRenderAPIFactory *> &FRenderAPIManager::GetAvailableFactoryList() {
    static TArray<FRenderAPIFactory *> inst;
    return inst;
}

FRenderWindow *FRenderAPIManager::initialize(const FString &pluginName, const FRenderWindowDesc &desc) {
    if (mRenderAPIInitialized) {
        return nullptr;
    }

#if PLATFORM != PLATFORM_ANDROID
    auto *lib = gDynLibManager().load(pluginName);
    FString name;

    if (lib != nullptr) {
        typedef const TCHAR *(*GetPluginNameFunc)();
        auto func = (GetPluginNameFunc) lib->getSymbol("getPluginName");
        if (func != nullptr) {
            name = func();
        }
    }
#else
    FString name = pluginName;
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

FRenderAPIManager &gRenderAPIManager() {
    return FRenderAPIManager::Instance();
}
