#pragma once

#include "CorePrerequisites.h"
#include "ViewInfo.h"

struct SceneData {
    TArray<RenderTargetInfo> renderTargetInfos;
    TArray<ViewInfo *> views;
    TMap<CameraBase *, uint32_t> cameraToView;

    TArray<RenderableInfo *> renderables;
    TArray<RenderableLight *> lights;

    GpuBuffer *gLightsBuffer;
};

class DLL_EXPORT SceneInfo {
private:
    SceneData mData;

public:
    SceneInfo();
    ~SceneInfo();

public:
    void registerCamera(CameraBase *camera);
    void updateCamera(CameraBase *camera, uint32_t updateFlags);
    void unregisterCamera(CameraBase *camera);

    void registerRenderable(Renderable *renderable);
    void updateRenderable(Renderable *renderable, uint32_t updateFlags);
    void unregisterRenderable(Renderable *renderable);

    void registerLight(LightBase *light);
    void updateLight(LightBase *light, uint32_t updateFlags);
    void unregisterLight(LightBase *light);

    const SceneData &getSceneData() const { return mData; }

private:
    ViewInfoDesc createViewInfoDesc(CameraBase *camera) const;
    void updateCameraRenderTargets(CameraBase *camera, bool remove = false);
};
