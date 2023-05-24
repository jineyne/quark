#include "CameraComponent.h"
#include "Scene/Actor.h"

void FCameraComponent::onCreate() {
    mInternal = q_new<FCameraBase>();
    mInternal->setTransform(getOwner()->getTransform());

    mViewport = mInternal->getViewport();
    mRenderSettings = mInternal->getRenderSettings();

    mInternal->initialize();
}

void FCameraComponent::onStart() { }

void FCameraComponent::onUpdate() {
    if (mInternal->isDirty()) {
        mInternal->update(EActorDirtyFlags::Everything);
    }
}

void FCameraComponent::onActive() {
    mInternal->setLayers(mLayers);
    mInternal->setProjectionType(mProjectionType);
    mInternal->setHorzFov(mHorzFov);
    mInternal->setFarClipDistance(mFarDist);
    mInternal->setNearClipDistance(mNearDist);
    mInternal->setAspectRatio(mAspect);
    mInternal->setOrthoWindowHeight(mOrthHeight);
    mInternal->setOrthoWindowWidth(mOrthHeight * mAspect);
    mInternal->setPriority(mPriority);
    mInternal->setMain(bMain);

    mInternal->update(EActorDirtyFlags::Active);
}

void FCameraComponent::onDeactive() {
    mInternal->update(EActorDirtyFlags::Active);
}

void FCameraComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    if ((flags & ETransformChangedFlags::Transform) == ETransformChangedFlags::Transform) {
        mInternal->update(EActorDirtyFlags::Transform);
    }

    mInternal->setDirty();
}

FVector3 FCameraComponent::screenToWorldPoint(const FVector2 &screenPoint, float depth) const {
    return mInternal->screenToWorldPoint(screenPoint, depth);
}

FVector2 FCameraComponent::screenToNdcPoint(const FVector2 &screenPoint) const {
    return mInternal->screenToNdcPoint(screenPoint);
}

FVector3 FCameraComponent::ndcToWorldPoint(const FVector2 &ndcPoint, float depth) const {
    return mInternal->ndcToWorldPoint(ndcPoint, depth);
}

void FCameraComponent::setLayers(const uint64_t &layers) {
    mLayers = layers;
    if (isActive()) {
        mInternal->setLayers(layers);
    }
}

void FCameraComponent::setHorzFov(const FRadian &fovy) {
    mHorzFov = fovy;

    if (isActive()) {
        mInternal->setHorzFov(fovy);
    }
}

void FCameraComponent::setFarClipDistance(float farDist) {
    mFarDist = farDist;

    if (isActive()) {
        mInternal->setFarClipDistance(farDist);
    }
}

void FCameraComponent::setNearClipDistance(float nearDist) {
    mNearDist = nearDist;

    if (isActive()) {
        mInternal->setNearClipDistance(nearDist);
    }
}

void FCameraComponent::setAspectRatio(float ratio) {
    mAspect = ratio;

    if (isActive()) {
        mInternal->setAspectRatio(ratio);
    }
}

void FCameraComponent::setProjectionType(EProjectionType type) {
    mProjectionType = type;

    if (isActive()) {
        mInternal->setProjectionType(type);
    }
}

void FCameraComponent::setOrthoWindow(float width, float height) {
    mOrthHeight = height;
    mAspect = width / height;

    if (isActive()) {
        mInternal->setOrthoWindow(width, height);
    }
}

void FCameraComponent::setOrthoWindowHeight(float height) {
    mOrthHeight = height;
    if (isActive()) {
        mInternal->setOrthoWindowHeight(height);
    }
}

void FCameraComponent::setOrthoWindowWidth(float width) {
    mOrthHeight = width / mAspect;

    if (isActive()) {
        mInternal->setOrthoWindowWidth(width);
    }
}

void FCameraComponent::setPriority(int32_t priority) {
    mPriority = priority;

    if (isActive()) {
        mInternal->setPriority(priority);
    }
}

void FCameraComponent::setMain(bool main) {
    bMain = main;

    if (isActive()) {
        mInternal->setMain(main);
    }
}
