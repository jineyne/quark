#pragma once

#include "CorePrerequisites.h"
#include "Misc/Module.h"
#include "RenderAPIFactory.h"
#include "RenderWindow.h"
#include "RenderAPIManager.g.h"

QCLASS()
class DLL_EXPORT FRenderAPIManager : public TModule<FRenderAPIManager> {
    GENERATED_BODY();

private:
    bool mRenderAPIInitialized = false;

public:
    FRenderAPIManager() = default;
    ~FRenderAPIManager();

public:
    static void RegisterFactory(FRenderAPIFactory *factory);

private:
    static TArray<FRenderAPIFactory *> &GetAvailableFactoryList();

public:
    FRenderWindow *initialize(const FString &pluginName, const FRenderWindowDesc &desc);
};

DLL_EXPORT FRenderAPIManager &gRenderAPIManager();