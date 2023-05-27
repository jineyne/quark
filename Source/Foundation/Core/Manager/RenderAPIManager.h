#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/RenderAPIFactory.h"
#include "RenderAPI/RenderWindow.h"
#include "Misc/Module.h"
#include "RenderAPIManager.g.h"

QCLASS()
class DLL_EXPORT RenderAPIManager : public TModule<RenderAPIManager> {
    GENERATED_BODY();

private:
    bool mRenderAPIInitialized = false;

public:
    RenderAPIManager() = default;
    ~RenderAPIManager();

public:
    static void RegisterFactory(RenderAPIFactory *factory);

private:
    static TArray<RenderAPIFactory *> &GetAvailableFactoryList();

public:
    RenderWindow *initialize(const String &pluginName, const RenderWindowDesc &desc);
};

DLL_EXPORT RenderAPIManager &gRenderAPIManager();