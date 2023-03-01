#include "CoreApplication.h"

#include "RenderAPI/RenderAPIManager.h"
#include "RenderAPI/RenderWindowManager.h"
#include "Plugin/DynLibManager.h"
#include "Plugin/PluginManager.h"
#include "Misc/Time.h"
#include "RenderAPI/RenderAPI.h"

QCoreApplication::QCoreApplication(const FApplicationStartUpDesc &desc) : mDesc(desc) {}

void QCoreApplication::runMainLoop() {
    bMainLoopRunning = true;

    while (isMainLoopRunning()) {
        if (bDisplayInitialized) {
            mainFrame();
        }
    }
}

void QCoreApplication::onDisplayInit() {
    bDisplayInitialized = true;

    mPrimaryWindow = gRenderAPIManager().initialize(mDesc.renderAPI, mDesc.primaryWindowDesc);

    for (auto importer : mDesc.importers) {
        // Importer::Instance().registerAssetImporter(loadPlugin(importer));
    }
}

void QCoreApplication::mainFrame() {
    calculateFrameStats();

    gTime().update();

    gRenderWindowManager().update();

    gRenderAPI().setRenderTarget(mPrimaryWindow);
    gRenderAPI().clearRenderTarget(EFrameBufferType::Color);
    gRenderAPI().swapBuffer(mPrimaryWindow);
}

void QCoreApplication::quitRequest() {

}

void QCoreApplication::setIsMainLoopRunning(bool isRunning) {

}

void QCoreApplication::onStartUp() {
    FTime::StartUp();

    FDynLibManager::StartUp();
    FPluginManager::StartUp();

    FRenderAPIManager::StartUp();

    onDisplayInit();
}

void QCoreApplication::onShutDown() {
    mPrimaryWindow = nullptr;

    FRenderAPIManager::ShutDown();

    FPluginManager::ShutDown();
    FDynLibManager::ShutDown();

    FTime::ShutDown();
}

void QCoreApplication::calculateFrameStats() {
    static int frameCnt = 0;
    static float timeElapsed = 0.0f;

    frameCnt++;
    timeElapsed += gTime().getDeltaTime();
    if (timeElapsed >= 1.0f) {
        float fps = (float) frameCnt;
        float mspf = 1000.0f / fps;

        mPrimaryWindow->setTitle(FString::Printf(TEXT("FPS: %lf\tFrame Time: %lf"), fps, mspf));

        frameCnt = 0;
        timeElapsed = 0;
    }
}


QCoreApplication &gCoreApplication() {
    return QCoreApplication::Instance();
}
