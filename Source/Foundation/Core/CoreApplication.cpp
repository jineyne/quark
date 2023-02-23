#include "CoreApplication.h"

#include "RenderAPI/RenderAPIManager.h"
#include "Plugin/DynLibManager.h"
#include "Plugin/PluginManager.h"

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

}

void QCoreApplication::quitRequest() {

}

void QCoreApplication::setIsMainLoopRunning(bool isRunning) {

}

void QCoreApplication::onStartUp() {
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
}

QCoreApplication &gCoreApplication() {
    return QCoreApplication::Instance();
}
