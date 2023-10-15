#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/RenderWindow.h"
#include "Misc/Module.h"
#include "CoreApplication.g.h"

struct ApplicationStartUpDesc {
    RenderWindowDesc primaryWindowDesc;
    String renderAPI;
    String physicsAPI;
    TArray<String> importers;
};

QCLASS()
class DLL_EXPORT CoreApplication : public TModule<CoreApplication> {
    GENERATED_BODY();

private:
    ApplicationStartUpDesc mDesc;
    RenderWindow *mPrimaryWindow = nullptr;

    float mFrameStep = 1000.f / 60.f;
    float mLastFrameTime;
    int mFrameCount = 0;
    float mFrameTime = 0;

    bool bMainLoopRunning = false;
    bool bDisplayInitialized = false;

public:
    CoreApplication() = default;

    CoreApplication(const ApplicationStartUpDesc &desc);
    virtual ~CoreApplication() = default;

public:
    virtual void runMainLoop();
    virtual void onDisplayInit();

    void mainFrame();
    void quitRequest();

    void fixedUpdate();

    void setIsMainLoopRunning(bool isRunning);
    auto isMainLoopRunning() const { return bMainLoopRunning; }
    auto getPrimaryWindow() const { return mPrimaryWindow; }

protected:
    void onStartUp() override;
    void onShutDown() override;

public: // TODO: TEST
    void calculateFrameStats();
};

DLL_EXPORT CoreApplication &gCoreApplication();