#include "RigidBodyComponent.h"

void RigidBodyComponent::onCreate() {
    mInternal = q_new<RigidBody>();
}

void RigidBodyComponent::onDestroy() {
    q_delete(mInternal);
}

void RigidBodyComponent::onActive() {
    mInternal->setMass(mMass);
    mInternal->setForce(mForce);
    mInternal->setVelocity(mVelocity);
    mInternal->setTransform(getTransform());

    mInternal->update(EActorDirtyFlags::Active);
}

void RigidBodyComponent::onDeactive() {
    mInternal->update(EActorDirtyFlags::Active);
}
