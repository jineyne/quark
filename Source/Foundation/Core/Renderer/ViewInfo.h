#pragma once

#include "CorePrerequisites.h"
#include "Image/Color.h"
#include "CameraBase.h"
#include "RenderQueue.h"
#include "RenderableInfo.h"
#include "RenderableLight.h"

struct FSceneData;

struct FViewTargetData {
    FRenderTarget *target;
    FRect viewRect;
    uint32_t targetWidth;
    uint32_t targetHeight;

    uint32_t clearFlags;
    FColor clearColor;
};

struct FViewInfoDesc {
    FMatrix4 viewTransform;
    FMatrix4 projTransform;
    FViewTargetData target;
    EStateReduction stateReduction;
    FCameraBase *camera;
    EProjectionType projType;
};

struct FRenderTargetInfo {
    FRenderTarget *target;
    TArray<FCameraBase *> cameras;
};

struct FVisibilityInfo {
    std::vector<bool> renderables;
    std::vector<bool> lights;
};

class DLL_EXPORT FViewInfo {
private:
    FViewTargetData mTarget;
    FCameraBase *mCamera = nullptr;
    FRenderQueue *mOpaqueQueue = nullptr;
    FVisibilityInfo mVisibility;

    bool mRedrawThisFrame = false;
    float mRedrawForSeconds = 0.0f;
    uint32_t mRedrawForFrames = 0;
    FMatrix4 mViewProjTransform;

public:
    FViewInfo();
    FViewInfo(const FViewInfoDesc &desc);
    ~FViewInfo();

public:
    void setStateReductionMode(EStateReduction reductionMode);
    void setView(const FViewInfoDesc &desc);

    void beginFrame();
    void endFrame();
    void determineVisible(const TArray<FRenderableInfo *> &renderables, std::vector<bool> *visibility = nullptr);
    void determineVisible(const TArray<FRenderableLight *> &lights, std::vector<bool> *visibility = nullptr);
    void queueRenderElements(const FSceneData &sceneData);

    void setTransform(const FVector3& origin, const FVector3& direction, const FMatrix4& view, const FMatrix4& proj);

    const FVisibilityInfo &getVisibilityMasks() const { return mVisibility; }
    FRenderQueue *getOpaqueQueue() const { return mOpaqueQueue; }
    FViewTargetData getTargetData() const { return mTarget; }
    FCameraBase *getSceneCamera() const { return mCamera; }
    const FMatrix4 &getViewProjTransform() const { return mViewProjTransform; }
};

class FViewInfoGroup {
private:
    TArray<FViewInfo *> mViewList;

    FVisibilityInfo mVisibility;

    bool mMainPass = false;

public:
    FViewInfoGroup(FViewInfo **views, uint32_t viewCount, bool mainPass);

public:
    void setViews(FViewInfo **views, uint32_t viewCount);
    FViewInfo *getView(uint32_t idx) { return mViewList[idx]; }

    bool isMainPass() const { return mMainPass; }
    const FVisibilityInfo &getVisibilityInfo() const { return mVisibility; }
    void determineVisibility(const FSceneData &sceneData);
};