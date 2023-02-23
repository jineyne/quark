#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/RenderWindow.h"
#include "Misc/Module.h"
#include "CoreApplication.g.h"

struct FApplicationStartUpDesc {
    FRenderWindowDesc primaryWindowDesc;
    FString renderAPI;
    TArray<FString> importers;
};

QCLASS()
class DLL_EXPORT QCoreApplication : public TModule<QCoreApplication> {
    GENERATED_BODY();

private:
    FApplicationStartUpDesc mDesc;
    FRenderWindow *mPrimaryWindow = nullptr;

    bool bMainLoopRunning = false;
    bool bDisplayInitialized = false;

public:
    QCoreApplication() = default;

    QCoreApplication(const FApplicationStartUpDesc &desc);
    virtual ~QCoreApplication() = default;

public:
    virtual void runMainLoop();
    virtual void onDisplayInit();

    void mainFrame();
    void quitRequest();

    void setIsMainLoopRunning(bool isRunning);
    auto isMainLoopRunning() const { return bMainLoopRunning; }
    auto getPrimaryWindow() const { return mPrimaryWindow; }

protected:
    void onStartUp() override;
    void onShutDown() override;
};

DLL_EXPORT QCoreApplication &gCoreApplication();