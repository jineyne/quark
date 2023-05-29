#include "CoreApplication.h"

#include "Importer/Importer.h"
#include "Manager/MeshManager.h"
#include "Manager/InputManager.h"
#include "Manager/RenderAPIManager.h"
#include "Manager/RenderWindowManager.h"
#include "Manager/SceneManager.h"
#include "Manager/SceneObjectManager.h"
#include "Physics/Physics.h"
#include "Renderer/Renderer.h"
#include "Resource/Resources.h"
#include "Plugin/DynLibManager.h"
#include "Plugin/PluginManager.h"
#include "Misc/Time.h"
#include "RenderAPI/RenderAPI.h"

CoreApplication::CoreApplication(const ApplicationStartUpDesc &desc) : mDesc(desc) {}

void CoreApplication::runMainLoop() {
    bMainLoopRunning = true;

    while (isMainLoopRunning()) {
        if (bDisplayInitialized) {
            mainFrame();
        }
    }
}

void CoreApplication::onDisplayInit() {
    bDisplayInitialized = true;

    mPrimaryWindow = gRenderAPIManager().initialize(mDesc.renderAPI, mDesc.primaryWindowDesc);

    ParamBlockManager::StartUp();

    Renderer::StartUp();
    Physics::StartUp();

    Importer::StartUp();

    MeshManager::StartUp();

    SceneObjectManager::StartUp();
    SceneManager::StartUp();

    for (auto importer : mDesc.importers) {
        gPluginManager().loadPlugin(importer);
    }
}

void CoreApplication::mainFrame() {
    calculateFrameStats();

    gTime().update();

    gRenderWindowManager().update();

    gSceneManager().update();

    {
        uint64_t step;
        auto iteration = gTime().getFixedUpdateStep(step);
        const float stepSeconds = step / 1000000.0f;

        for (uint32_t i = 0; i < iteration; i++) {
            fixedUpdate();
            gSceneManager().fixedUpdate();
            gPhysics().fixedUpdate(stepSeconds);

            gTime().advanceFixedUpdate(step);
        }
    }

    gRenderer().renderAll();
    gInputManager().update();
}

void CoreApplication::quitRequest() {
    bMainLoopRunning = false;
}

void CoreApplication::fixedUpdate() {
}

void CoreApplication::setIsMainLoopRunning(bool isRunning) {
    bMainLoopRunning = isRunning;
}

void CoreApplication::onStartUp() {
    Platform::Initialize();

    Time::StartUp();

    DynLibManager::StartUp();
    PluginManager::StartUp();

    RenderAPIManager::StartUp();

    Resources::StartUp();

    onDisplayInit();
}

void CoreApplication::onShutDown() {
    mPrimaryWindow = nullptr;

    SceneManager::ShutDown();
    SceneObjectManager::ShutDown();

    MeshManager::ShutDown();

    Importer::ShutDown();

    Physics::ShutDown();
    Renderer::ShutDown();
    ParamBlockManager::ShutDown();

    InputManager::ShutDown();

    Resources::ShutDown();
    RenderAPIManager::ShutDown();

    PluginManager::ShutDown();
    DynLibManager::ShutDown();

    Time::ShutDown();
}

void CoreApplication::calculateFrameStats() {
    static int frameCnt = 0;
    static float timeElapsed = 0.0f;

    frameCnt++;
    timeElapsed += gTime().getDeltaTime();
    if (timeElapsed >= 1.0f) {
        float fps = (float) frameCnt;
        float mspf = 1000.0f / fps;

        mPrimaryWindow->setTitle(String::Printf(TEXT("FPS: %lf Frame Time: %lf"), fps, mspf));

        frameCnt = 0;
        timeElapsed = 0;
    }
}


CoreApplication &gCoreApplication() {
    return CoreApplication::Instance();
}
