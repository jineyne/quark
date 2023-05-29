#include "RigidBody.h"
#include "Physics.h"

RigidBody::RigidBody() {

}

RigidBody::~RigidBody() {
    gPhysics().notifyRigidBodyRemoved(this);
}

void RigidBody::initialize() {
    gPhysics().notifyRigidBodyCreated(this);
}

void RigidBody::update(EActorDirtyFlags flags) {
    EActorDirtyFlags updateEverythingFlag = EActorDirtyFlags::Everything | EActorDirtyFlags::Active;

    if ((flags & updateEverythingFlag) != EActorDirtyFlags::None) {
        if (bIsActiveOld != bIsActive) {
            if (bIsActive)
                gPhysics().notifyRigidBodyCreated(this);
            else
                gPhysics().notifyRigidBodyRemoved(this);
        } else {
            gPhysics().notifyRigidBodyRemoved(this);
            gPhysics().notifyRigidBodyCreated(this);
        }

        bIsActiveOld = bIsActive;
    } else if ((flags & EActorDirtyFlags::Mobility) != EActorDirtyFlags::None) {
        gPhysics().notifyRigidBodyRemoved(this);
        gPhysics().notifyRigidBodyCreated(this);
    } else if ((flags & EActorDirtyFlags::Transform) != EActorDirtyFlags::None) {
        if (bIsActive) {
            gPhysics().notifyRigidBodyRemoved(this);
            gPhysics().notifyRigidBodyCreated(this);
        }
    }
}

Transform *RigidBody::getTransform() const {
    return mTransform;
}

void RigidBody::setTransform(Transform *transform) {
    mTransform = transform;
}

const Vector3 &RigidBody::getVelocity() const {
    return mVelocity;
}

void RigidBody::setVelocity(const Vector3 &velocity) {
    mVelocity = velocity;
}

const Vector3 &RigidBody::getForce() const {
    return mForce;
}

void RigidBody::setForce(const Vector3 &force) {
    mForce = force;
}

float RigidBody::getMass() const {
    return mMass;
}

void RigidBody::setMass(float mass) {
    mMass = mass;
}
