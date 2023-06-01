#include "RigidBodyComponent.h"
#include "Scene/Transform.h"

void RigidBodyComponent::onCreate() {
    mInternal = q_new<RigidBody>();
    mInternal->setTransform(getTransform());

    mInternal->initialize();
}

void RigidBodyComponent::onDestroy() {
    q_delete(mInternal);
}

void RigidBodyComponent::onActive() {
    mInternal->setMass(mMass);
    mInternal->setForce(mForce);
    mInternal->setVelocity(mVelocity);

    mInternal->updateData(EActorDirtyFlags::Active);
}

void RigidBodyComponent::onDeactive() {
    if (!isDestroyed()) {
        mInternal->updateData(EActorDirtyFlags::Active);
    }
}

void RigidBodyComponent::addForce(Vector3 force) {
    setForce(mInternal->getForce() + force);
}

void RigidBodyComponent::setVelocity(const Vector3 &velocity) {
    mVelocity = velocity;

    if (isActive()) {
        mInternal->setVelocity(velocity);
    }
}

void RigidBodyComponent::setForce(const Vector3 &force) {
    mForce = force;

    if (isActive()) {
        mInternal->setForce(force);
    }
}

void RigidBodyComponent::setMass(float mass) {
    mMass = mass;

    if (isActive()) {
        mInternal->setMass(mass);
    }
}
