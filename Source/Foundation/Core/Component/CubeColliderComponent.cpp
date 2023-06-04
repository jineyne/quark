#include "CubeColliderComponent.h"
#include "Scene/Actor.h"
#include "Scene/Transform.h"

void CubeColliderComponent::onCreate() {
    mInternal = q_new<CubeCollider>();
    mInternal->setTransform(getTransform());

    mInternal->CollisionEnter.bindDynamic(CubeColliderComponent::onCollisionEnter);
    mInternal->CollisionStay.bindDynamic(CubeColliderComponent::onCollisionStay);
    mInternal->CollisionExit.bindDynamic(CubeColliderComponent::onCollisionExit);

    mInternal->initialize();
}

void CubeColliderComponent::onDestroy() {
    q_delete(mInternal);
}

void CubeColliderComponent::onFixedUpdate() {
    if (mInternal->isDirty()) {
        mInternal->updateData(EActorDirtyFlags::Transform);
    }
}

void CubeColliderComponent::onActive() {
    mInternal->setHalfSize(mHalfSize);
    mInternal->setOffset(mOffset);
    mInternal->setIsTrigger(mIsTrigger);

    if (!isDestroyed()) {
        mInternal->setActive(true);
        mInternal->updateData(EActorDirtyFlags::Active);
    }
}

void CubeColliderComponent::onDeactive() {
    if (!isDestroyed()) {
        mInternal->setActive(false);
        mInternal->updateData(EActorDirtyFlags::Active);
    }
}

void CubeColliderComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    mInternal->updateData(EActorDirtyFlags::Transform);
}

const Vector3 &CubeColliderComponent::getHalfSize() const {
    return mHalfSize;
}

void CubeColliderComponent::setHalfSize(const Vector3 &halfSize) {
    mHalfSize = halfSize;

    if (isActive()) {
        mInternal->setHalfSize(halfSize);
    }
}

const Vector3 &CubeColliderComponent::getOffset() const {
    return mOffset;
}

void CubeColliderComponent::setOffset(const Vector3 &offset) {
    mOffset = offset;

    if (isActive()) {
        mInternal->setOffset(offset);
    }
}

bool CubeColliderComponent::isTrigger() const {
    return mIsTrigger;
}

void CubeColliderComponent::setIsTrigger(bool isTrigger) {
    mIsTrigger = isTrigger;

    if (isActive()) {
        mInternal->setIsTrigger(isTrigger);
    }
}

void CubeColliderComponent::onCollisionEnter(Collider *other) {
    if (CollisionEnter != nullptr) {
        CollisionEnter(other);
    }
}

void CubeColliderComponent::onCollisionStay(Collider *other) {
    if (CollisionStay != nullptr) {
        CollisionStay(other);
    }
}

void CubeColliderComponent::onCollisionExit(Collider *other) {
    if (CollisionExit != nullptr) {
        CollisionExit(other);
    }
}
