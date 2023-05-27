#pragma once

#include "CorePrerequisites.h"
#include "Image/Color.h"
#include "CameraBase.h"
#include "RenderQueue.h"
#include "RenderableInfo.h"
#include "RenderableLight.h"

struct SceneData;

struct ViewTargetData {
    RenderTarget *target;
    Rect viewRect;
    uint32_t targetWidth;
    uint32_t targetHeight;

    uint32_t clearFlags;
    Color clearColor;
};

struct ViewInfoDesc {
    Matrix4 viewTransform;
    Matrix4 projTransform;
    ViewTargetData target;
    EStateReduction stateReduction;
    CameraBase *camera;
    EProjectionType projType;
};

struct RenderTargetInfo {
    RenderTarget *target;
    TArray<CameraBase *> cameras;
};

struct VisibilityInfo {
    std::vector<bool> renderables;
    std::vector<bool> lights;
};

class DLL_EXPORT ViewInfo {
private:
    ViewTargetData mTarget;
    CameraBase *mCamera = nullptr;
    RenderQueue *mOpaqueQueue = nullptr;
    VisibilityInfo mVisibility;

    bool mRedrawThisFrame = false;
    float mRedrawForSeconds = 0.0f;
    uint32_t mRedrawForFrames = 0;
    Matrix4 mViewProjTransform;

public:
    ViewInfo();
    ViewInfo(const ViewInfoDesc &desc);
    ~ViewInfo();

public:
    void setStateReductionMode(EStateReduction reductionMode);
    void setView(const ViewInfoDesc &desc);

    void beginFrame();
    void endFrame();
    void determineVisible(const TArray<RenderableInfo *> &renderables, std::vector<bool> *visibility = nullptr);
    void determineVisible(const TArray<RenderableLight *> &lights, std::vector<bool> *visibility = nullptr);
    void queueRenderElements(const SceneData &sceneData);

    void setTransform(const Vector3& origin, const Vector3& direction, const Matrix4& view, const Matrix4& proj);

    const VisibilityInfo &getVisibilityMasks() const { return mVisibility; }
    RenderQueue *getOpaqueQueue() const { return mOpaqueQueue; }
    ViewTargetData getTargetData() const { return mTarget; }
    CameraBase *getSceneCamera() const { return mCamera; }
    const Matrix4 &getViewProjTransform() const { return mViewProjTransform; }
};

class ViewInfoGroup {
private:
    TArray<ViewInfo *> mViewList;

    VisibilityInfo mVisibility;

    bool mMainPass = false;

public:
    ViewInfoGroup(ViewInfo **views, uint32_t viewCount, bool mainPass);

public:
    void setViews(ViewInfo **views, uint32_t viewCount);
    ViewInfo *getView(uint32_t idx) { return mViewList[idx]; }

    bool isMainPass() const { return mMainPass; }
    const VisibilityInfo &getVisibilityInfo() const { return mVisibility; }
    void determineVisibility(const SceneData &sceneData);
};