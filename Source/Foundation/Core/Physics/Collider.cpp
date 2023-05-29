#include "Collider.h"
#include "Physics.h"

Collider::~Collider() {
    gPhysics().notifyColliderRemoved(this);
}

void Collider::initialize() {
    gPhysics().notifyColliderCreated(this);
}

void Collider::update(EActorDirtyFlags flags) {
    EActorDirtyFlags updateEverythingFlag = EActorDirtyFlags::Everything | EActorDirtyFlags::Active;

    if ((flags & updateEverythingFlag) != EActorDirtyFlags::None) {
        if (bIsActiveOld != bIsActive) {
            if (bIsActive)
                gPhysics().notifyColliderCreated(this);
            else
                gPhysics().notifyColliderRemoved(this);
        } else {
            gPhysics().notifyColliderRemoved(this);
            gPhysics().notifyColliderCreated(this);
        }

        bIsActiveOld = bIsActive;
    } else if ((flags & EActorDirtyFlags::Mobility) != EActorDirtyFlags::None) {
        gPhysics().notifyColliderRemoved(this);
        gPhysics().notifyColliderCreated(this);
    } else if ((flags & EActorDirtyFlags::Transform) != EActorDirtyFlags::None) {
        if (bIsActive) {
            gPhysics().notifyColliderUpdated(this);
        }
    }
}

bool Collider::isActive() const {
    return bIsActive;
}

void Collider::setActive(bool isActive) {
    bIsActive = isActive;
}

Transform *Collider::getTransform() const {
    return mTransform;
}

void Collider::setTransform(Transform *transform) {
    mTransform = transform;
}

const Vector3 &Collider::getOffset() const {
    return mOffset;
}

void Collider::setOffset(const Vector3 &offset) {
    mOffset = offset;
}

Vector3 Collider::getCenter() const {
    return mTransform->getPosition() + mOffset;
}

uint64_t Collider::getPhysicsId() const {
    return mPhysicsId;
}

void Collider::setPhysicsId(uint64_t physicsId) {
    mPhysicsId = physicsId;
}

const AABB &Collider::getRegisteredBounds() const {
    return mRegisteredBounds;
}

void Collider::setRegisteredBounds(const AABB &registeredBounds) {
    mRegisteredBounds = registeredBounds;
}
