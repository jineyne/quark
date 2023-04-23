#pragma once

#include "CorePrerequisites.h"
#include "ViewInfo.h"

struct FSceneData {
    TArray<FRenderTargetInfo> renderTargetInfos;
    TArray<FViewInfo *> views;
    TMap<FCameraBase *, uint32_t> cameraToView;

    TArray<FRenderableInfo *> renderables;
    TArray<FRenderableLight *> lights;

    FGpuBuffer *gLightsBuffer;
};

class DLL_EXPORT FSceneInfo {
private:
    FSceneData mData;

public:
    FSceneInfo();
    ~FSceneInfo();

public:
    void registerCamera(FCameraBase *camera);
    void updateCamera(FCameraBase *camera, uint32_t updateFlags);
    void unregisterCamera(FCameraBase *camera);

    void registerRenderable(FRenderable *renderable);
    void updateRenderable(FRenderable *renderable, uint32_t updateFlags);
    void unregisterRenderable(FRenderable *renderable);

    void registerLight(FLightBase *light);
    void updateLight(FLightBase *light, uint32_t updateFlags);
    void unregisterLight(FLightBase *light);

    const FSceneData &getSceneData() const { return mData; }

private:
    FViewInfoDesc createViewInfoDesc(FCameraBase *camera) const;
    void updateCameraRenderTargets(FCameraBase *camera, bool remove = false);
};
