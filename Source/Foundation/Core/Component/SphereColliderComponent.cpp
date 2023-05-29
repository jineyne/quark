#include "SphereColliderComponent.h"
#include "Scene/Actor.h"
#include "Scene/Transform.h"

void SphereColliderComponent::onCreate() {
    mInternal = q_new<SphereCollider>();
    mInternal->setTransform(getOwner()->getTransform());

    mInternal->initialize();
}

void SphereColliderComponent::onDestroy() {
    q_delete(mInternal);
}

void SphereColliderComponent::onActive() {
    mInternal->setRadius(mRadius);
    mInternal->setOffset(mOffset);

    mInternal->setActive(true);
    mInternal->update(EActorDirtyFlags::Active);
}

void SphereColliderComponent::onDeactive() {
    if (!isDestroyed()) {
        mInternal->setActive(false);
        mInternal->update(EActorDirtyFlags::Active);
    }
}

void SphereColliderComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    mInternal->update(EActorDirtyFlags::Transform);
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
