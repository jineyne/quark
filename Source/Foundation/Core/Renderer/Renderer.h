#pragma once

#include "CorePrerequisites.h"
#include "Misc/Module.h"
#include "Renderable.h"
#include "CameraBase.h"
#include "LightBase.h"
#include "SceneInfo.h"
#include "RendererExtension.h"
#include "Renderer.g.h"

DECLARE_LOG_CATEGORY_EXTERN(FLogRenderer, Debug)

QCLASS()
class DLL_EXPORT FRenderer : public TModule<FRenderer> {
    GENERATED_BODY();

private:
    struct RenderableInfo {
        FRenderable *origin = nullptr;
        FMesh *mesh = nullptr;
        FMaterial *material = nullptr;
        FGpuParamsSet *params = nullptr;
    };

private:
    FSceneInfo *mSceneInfo = nullptr;
    FViewInfoGroup *mMainViewGroup = nullptr;

    FViewport *mActiveViewport;
    std::set<FRendererExtension*, std::function<bool(const FRendererExtension*, const FRendererExtension*)>> mCallbacks;

public:
    FRenderer();

private:
    static bool CompareCallback(const FRendererExtension* a, const FRendererExtension* b);

public:
    void update();
    void renderAll();

    void notifyRenderableCreated(FRenderable *renderable);
    void notifyRenderableUpdated(FRenderable *renderable);
    void notifyRenderableRemoved(FRenderable *renderable);

    void notifyLightCreated(FLightBase *light);
    void notifyLightUpdated(FLightBase *light);
    void notifyLightRemoved(FLightBase *light);

    void notifyCameraCreated(FCameraBase *camera);
    void notifyCameraUpdated(FCameraBase *camera);
    void notifyCameraRemoved(FCameraBase *camera);

    void addPlugin(FRendererExtension *extension);
    void removePlugin(FRendererExtension *extension);

private:
    void updateCameraRenderTargets(FCameraBase *camera, bool removed);

    void renderOverlay(FCameraBase *camera);

    bool renderViews(TArray<FViewInfo *> &views);
    void renderView(FViewInfo *view);
};

DLL_EXPORT FRenderer &gRenderer();