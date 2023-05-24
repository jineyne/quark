#pragma once

#include "CorePrerequisites.h"
#include "RenderSettings.h"
#include "RenderAPI/Viewport.h"
#include "Math/Radian.h"
#include "Scene/Transform.h"

enum class ECameraFlags {
    OnDemand = 1 << 0
};

ENUM_CLASS_FLAGS(ECameraFlags)

enum class ECameraDirtyFlags {
    RenderSettings = 1 << 4,
    Redraw = 1 << 5,
    Viewport = 1 << 31
};

enum class EProjectionType {
    Perspective,
    Orthograhic,
};

class DLL_EXPORT FCameraBase {
private:
    static constexpr float InfiniteFarPlaneAdjust = 0.00001f;

    FRenderSettings *mRenderSettings;
    FViewport *mViewport = nullptr;
    uint64_t mLayers = 0;

    FTransform *mTransform = nullptr;

    EProjectionType mProjectionType = EProjectionType::Perspective;
    FRadian mHorzFov = FDegree(90.0f);

    float mFarDist = 500.0f;
    float mNearDist = 0.05f;
    float mAspect = 1.33333333333333f;
    float mOrthHeight = 5;

    int32_t mPriority = 0;

    bool bMain = false;
    bool bInitialized = false;
    bool bIsActive = true;
    bool bIsActiveOld = true;

    mutable FMatrix4 mProjMatrix;
    mutable FMatrix4 mViewMatrix;

    bool bFrustumExtentsManuallySet = false;
    bool bCustomViewMatrix = false;
    bool bCustomProjMatrix = false;

    mutable bool bRecalcFrustum = true;
    mutable bool bRecalcFrustumPlanes = true;
    mutable bool bRecalcView = true;

    mutable float mLeft, mRight, mTop, mBottom;

    uint32_t mRendererId;

public:
    FCameraBase();
    virtual ~FCameraBase();

public:
    static FCameraBase *New();

public:
    void initialize();
    void update(EActorDirtyFlags flags);

    FVector3 screenToWorldPoint(const FVector2 &screenPoint, float depth = 0.5f) const;
    FVector2 screenToNdcPoint(const FVector2 &screenPoint) const;
    FVector3 ndcToWorldPoint(const FVector2 &ndcPoint, float depth = 0.5f) const;
    void setTransform(FTransform *transform);
    void setLayers(const uint64_t &layers);
    void setHorzFov(const FRadian &fovy);
    void setFarClipDistance(float farDist);
    void setNearClipDistance(float nearDist);
    void setAspectRatio(float ratio);
    void setPriority(int32_t priority);
    void setProjectionType(EProjectionType type);
    void setOrthoWindow(float width, float height);
    void setOrthoWindowHeight(float height);
    void setOrthoWindowWidth(float width);
    void setDirty(bool dirty = true);
    void setMain(bool main);

    const auto &getViewport() const { return mViewport; }
    const auto &getRenderSettings() const { return mRenderSettings; }
    const auto &getTransform() const { return mTransform; }

    auto getLayers() const { return mLayers; }
    auto getFarClipDistance() const { return mFarDist; }
    auto getNearClipDistance() const { return mNearDist; }
    auto getAspectRatio() const { return mAspect; }
    auto getPriority() const { return mPriority; }
    auto getOrthoWindowWidth() const { return mOrthHeight * mAspect; }
    auto getOrthoWindowHeight() const { return mOrthHeight; }
    auto getProjectionType() const { return mProjectionType; }
    const FMatrix4 &getProjectionMatrix() const;
    const FMatrix4 &getViewMatrix() const;
    bool isInitialized() const { return bInitialized; }
    bool isDirty() const { return bRecalcView; }
    bool isMain() const { return bMain; }

    void setRendererId(uint32_t id) { mRendererId = id; }
    uint32_t getRendererId() const { return mRendererId; }

private:
    virtual void calcProjectionParameters(float &left, float &right, float &bottom, float &top) const;
    virtual void updateFrustum() const;
    virtual void updateView() const;

    virtual bool isFrustumOutOfDate() const;
    void invalidateFrustum() const;
};