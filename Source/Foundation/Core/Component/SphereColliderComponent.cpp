#include "SphereColliderComponent.h"
#include "Scene/Actor.h"
#include "Scene/Transform.h"

void SphereColliderComponent::onCreate() {
    mInternal = q_new<SphereCollider>();
    mInternal->setTransform(getOwner()->getTransform());

    mInternal->CollisionEnter.bindDynamic(SphereColliderComponent::onCollisionEnter);
    mInternal->CollisionStay.bindDynamic(SphereColliderComponent::onCollisionStay);
    mInternal->CollisionExit.bindDynamic(SphereColliderComponent::onCollisionExit);

    mInternal->initialize();
}

void SphereColliderComponent::onDestroy() {
    q_delete(mInternal);
}

void SphereColliderComponent::onActive() {
    mInternal->setRadius(mRadius);
    mInternal->setOffset(mOffset);

    mInternal->setActive(true);
    mInternal->updateData(EActorDirtyFlags::Active);
}

void SphereColliderComponent::onDeactive() {
    if (!isDestroyed()) {
        mInternal->setActive(false);
        mInternal->updateData(EActorDirtyFlags::Active);
    }
}

void SphereColliderComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    mInternal->updateData(EActorDirtyFlags::Transform);
}

float SphereColliderComponent::getRadius() const {
    return mRadius;
}

void SphereColliderComponent::setRadius(float radius) {
    mRadius = radius;

    if (isActive()) {
        mInternal->setRadius(radius);
    }
}

const Vector3 &SphereColliderComponent::getOffset() const {
    return mOffset;
}

void SphereColliderComponent::setOffset(const Vector3 &offset) {
    mOffset = offset;

    if (isActive()) {
        mInternal->setOffset(offset);
    }
}

bool SphereColliderComponent::isTrigger() const {
    return mIsTrigger;
}

void SphereColliderComponent::setIsTrigger(bool isTrigger) {
    mIsTrigger = isTrigger;

    if (isActive()) {
        mInternal->setIsTrigger(isTrigger);
    }
}

void SphereColliderComponent::onCollisionEnter(Collider *other) {
    if (CollisionEnter != nullptr) {
        CollisionEnter(other);
    }
}

void SphereColliderComponent::onCollisionStay(Collider *other) {
    if (CollisionStay != nullptr) {
        CollisionStay(other);
    }
}

void SphereColliderComponent::onCollisionExit(Collider *other) {
    if (CollisionExit != nullptr) {
        CollisionExit(other);
    }
}
