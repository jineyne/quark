#include "CameraComponent.h"
#include "Scene/Actor.h"

CameraComponent *CameraComponent::Main = nullptr;

CameraComponent *CameraComponent::GetMainComponent() {
    return Main;
}

void CameraComponent::onCreate() {
    mInternal = q_new<CameraBase>();
    mInternal->setTransform(getOwner()->getTransform());

    mViewport = mInternal->getViewport();
    mRenderSettings = mInternal->getRenderSettings();

    mInternal->initialize();
}

void CameraComponent::onDestroy() {
    Component::onDestroy();

    q_delete(mInternal);
}

void CameraComponent::onStart() { }

void CameraComponent::onUpdate() {
    if (mInternal->isDirty()) {
        mInternal->updateData(EActorDirtyFlags::Everything);
    }
}

void CameraComponent::onActive() {
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

    // mInternal->setActive(true);
    mInternal->updateData(EActorDirtyFlags::Active);
}

void CameraComponent::onDeactive() {
    // mInternal->setActive(false);
    mInternal->updateData(EActorDirtyFlags::Active);
}

void CameraComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    if ((flags & ETransformChangedFlags::Transform) == ETransformChangedFlags::Transform) {
        mInternal->updateData(EActorDirtyFlags::Transform);
    }

    mInternal->setDirty();
}

Vector3 CameraComponent::screenToWorldPoint(const Vector2 &screenPoint, float depth) const {
    return mInternal->screenToWorldPoint(screenPoint, depth);
}

Vector2 CameraComponent::screenToNdcPoint(const Vector2 &screenPoint) const {
    return mInternal->screenToNdcPoint(screenPoint);
}

Vector3 CameraComponent::ndcToWorldPoint(const Vector2 &ndcPoint, float depth) const {
    return mInternal->ndcToWorldPoint(ndcPoint, depth);
}

void CameraComponent::setLayers(const uint64_t &layers) {
    mLayers = layers;
    if (isActive()) {
        mInternal->setLayers(layers);
    }
}

void CameraComponent::setHorzFov(const Radian &fovy) {
    mHorzFov = fovy;

    if (isActive()) {
        mInternal->setHorzFov(fovy);
    }
}

void CameraComponent::setFarClipDistance(float farDist) {
    mFarDist = farDist;

    if (isActive()) {
        mInternal->setFarClipDistance(farDist);
    }
}

void CameraComponent::setNearClipDistance(float nearDist) {
    mNearDist = nearDist;

    if (isActive()) {
        mInternal->setNearClipDistance(nearDist);
    }
}

void CameraComponent::setAspectRatio(float ratio) {
    mAspect = ratio;

    if (isActive()) {
        mInternal->setAspectRatio(ratio);
    }
}

void CameraComponent::setProjectionType(EProjectionType type) {
    mProjectionType = type;

    if (isActive()) {
        mInternal->setProjectionType(type);
    }
}

void CameraComponent::setOrthoWindow(float width, float height) {
    mOrthHeight = height;
    mAspect = width / height;

    if (isActive()) {
        mInternal->setOrthoWindow(width, height);
    }
}

void CameraComponent::setOrthoWindowHeight(float height) {
    mOrthHeight = height;
    if (isActive()) {
        mInternal->setOrthoWindowHeight(height);
    }
}

void CameraComponent::setOrthoWindowWidth(float width) {
    mOrthHeight = width / mAspect;

    if (isActive()) {
        mInternal->setOrthoWindowWidth(width);
    }
}

void CameraComponent::setPriority(int32_t priority) {
    mPriority = priority;

    if (isActive()) {
        mInternal->setPriority(priority);
    }
}

void CameraComponent::setMain(bool main) {
    bMain = main;

    if (bMain) {
        if (Main) {
            Main->setMain(false);
        }

        Main = this;
    }

    if (isActive()) {
        mInternal->setMain(main);
    }
}

