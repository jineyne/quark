#pragma once

#include "CorePrerequisites.h"
#include "Misc/Module.h"
#include "Renderable.h"
#include "CameraBase.h"
#include "LightBase.h"
#include "SceneInfo.h"
#include "RendererExtension.h"
#include "Renderer.g.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRenderer, Debug)

QCLASS()
class DLL_EXPORT Renderer : public TModule<Renderer> {
    GENERATED_BODY();

private:
    struct RenderableInfo {
        Renderable *origin = nullptr;
        Mesh *mesh = nullptr;
        Material *material = nullptr;
        GpuParamsSet *params = nullptr;
    };

private:
    SceneInfo *mSceneInfo = nullptr;
    ViewInfoGroup *mMainViewGroup = nullptr;

    Viewport *mActiveViewport;
    std::set<RendererExtension*, std::function<bool(const RendererExtension*, const RendererExtension*)>> mCallbacks;

public:
    Renderer();

private:
    static bool CompareCallback(const RendererExtension* a, const RendererExtension* b);

public:
    void update();
    void renderAll();

    void notifyRenderableCreated(Renderable *renderable);
    void notifyRenderableUpdated(Renderable *renderable);
    void notifyRenderableRemoved(Renderable *renderable);

    void notifyLightCreated(LightBase *light);
    void notifyLightUpdated(LightBase *light);
    void notifyLightRemoved(LightBase *light);

    void notifyCameraCreated(CameraBase *camera);
    void notifyCameraUpdated(CameraBase *camera);
    void notifyCameraRemoved(CameraBase *camera);

    void addPlugin(RendererExtension *extension);
    void removePlugin(RendererExtension *extension);

private:
    void updateCameraRenderTargets(CameraBase *camera, bool removed);

    void renderOverlay(CameraBase *camera);

    bool renderViews(TArray<ViewInfo *> &views);
    void renderView(ViewInfo *view);
};

DLL_EXPORT Renderer &gRenderer();