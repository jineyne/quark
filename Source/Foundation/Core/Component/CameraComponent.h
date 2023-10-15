#pragma once

#include "CorePrerequisites.h"
#include "Renderer/CameraBase.h"
#include "Scene/Component.h"
#include "CameraComponent.g.h"

QCLASS()
class DLL_EXPORT CameraComponent : public Component {
    GENERATED_BODY();

private:
    static CameraComponent *Main;

    CameraBase *mInternal;

    RenderSettings *mRenderSettings;
    Viewport *mViewport = nullptr;
    uint64_t mLayers = 0;

    EProjectionType mProjectionType = EProjectionType::Perspective;
    Radian mHorzFov = Degree(90.0f);

    float mFarDist = 500.0f;
    float mNearDist = 0.05f;
    float mAspect = 1.33333333333333f;
    float mOrthHeight = 5;

    int32_t mPriority = 0;
    bool bMain = false;

public:
    static CameraComponent *GetMainComponent();

public:
    void onCreate() override;

    void onDestroy() override;

    void onStart() override;
    void onUpdate() override;

    void onActive() override;
    void onDeactive() override;

    void onTransformChanged(const ETransformChangedFlags &flags) override;

    Vector3 screenToWorldPoint(const Vector2 &screenPoint, float depth = 0.5f) const;
    Vector2 screenToNdcPoint(const Vector2 &screenPoint) const;
    Vector3 ndcToWorldPoint(const Vector2 &ndcPoint, float depth = 0.5f) const;

    void setLayers(const uint64_t &layers);
    auto getLayers() const { return mLayers; }

    void setHorzFov(const Radian &fovy);

    void setFarClipDistance(float farDist);
    auto getFarClipDistance() const { return mFarDist; }

    void setNearClipDistance(float nearDist);
    auto getNearClipDistance() const { return mNearDist; }

    void setAspectRatio(float ratio);
    auto getAspectRatio() const { return mAspect; }

    void setProjectionType(EProjectionType type);
    auto getProjectionType() const { return mProjectionType; }

    void setOrthoWindow(float width, float height);

    void setOrthoWindowHeight(float height);
    auto getOrthoWindowHeight() const { return mOrthHeight; }

    void setOrthoWindowWidth(float width);
    auto getOrthoWindowWidth() const { return mOrthHeight * mAspect; }

    void setMain(bool main);

    void setPriority(int32_t priority);
    auto getPriority() const { return mInternal->getPriority(); }
    bool isMain() const { return mInternal->isMain(); }

    const auto &getViewport() const { return mInternal->getViewport(); }
    const auto &getRenderSettings() const { return mInternal->getRenderSettings(); }
};
