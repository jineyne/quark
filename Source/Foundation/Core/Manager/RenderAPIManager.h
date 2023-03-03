#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/RenderAPIFactory.h"
#include "RenderAPI/RenderWindow.h"
#include "Misc/Module.h"
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