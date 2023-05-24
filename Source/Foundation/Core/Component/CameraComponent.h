#pragma once

#include "CorePrerequisites.h"
#include "Renderer/CameraBase.h"
#include "Scene/Component.h"
#include "CameraComponent.g.h"

QCLASS()
class DLL_EXPORT FCameraComponent : public FComponent {
    GENERATED_BODY();

private:
    FCameraBase *mInternal;

    FRenderSettings *mRenderSettings;
    FViewport *mViewport = nullptr;
    uint64_t mLayers = 0;

    EProjectionType mProjectionType = EProjectionType::Perspective;
    FRadian mHorzFov = FDegree(90.0f);

    float mFarDist = 500.0f;
    float mNearDist = 0.05f;
    float mAspect = 1.33333333333333f;
    float mOrthHeight = 5;

    int32_t mPriority = 0;
    bool bMain = false;

public:
    void onCreate() override;

    void onStart() override;
    void onUpdate() override;

    void onActive() override;
    void onDeactive() override;

    void onTransformChanged(const ETransformChangedFlags &flags) override;

    FVector3 screenToWorldPoint(const FVector2 &screenPoint, float depth = 0.5f) const;
    FVector2 screenToNdcPoint(const FVector2 &screenPoint) const;
    FVector3 ndcToWorldPoint(const FVector2 &ndcPoint, float depth = 0.5f) const;

    void setLayers(const uint64_t &layers);
    auto getLayers() const { return mLayers; }

    void setHorzFov(const FRadian &fovy);

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
