#include "Physics.h"
#include "Scene/Transform.h"
#include "Misc/Time.h"

DEFINE_LOG_CATEGORY(LogPhysics)

void Physics::onStartUp() {
}

void Physics::onShutDown() {
}

void Physics::fixedUpdate(float st) {
    // apply velocity
    for (auto *body : mRegisteredRigidBodyList) {
        Transform *transform = body->getTransform();

        Vector3 velocity = body->getVelocity();
        Vector3 force = body->getForce();
        Vector3 position = transform->getPosition();
        auto mass = body->getMass();

        force += mGravity * body->getMass();
        velocity += (mass != 0 ? force / body->getMass() : force) * st;
        position += velocity * st;

        body->setForce(Vector3::ZeroVector);
        body->setVelocity(velocity);
        transform->setPosition(position);
    }

    for (auto *collider : mTriggerColliderList) {
        TArray<Collider *> founds = mTree.findCollisionObjects(collider);

        for (auto &other : founds) {
            auto points = collider->testCollision(collider->getTransform(), other, other->getTransform());

            if (!points.hasCollision) {
                continue;
            }

            collider->CollisionEnter(other);
        }
    }
}

void Physics::notifyRigidBodyCreated(RigidBody *body) {
#if DEBUG_MODE
    if (mRegisteredRigidBodyList.contains(body)) {
        LOG(LogPhysics, Warning, TEXT("Try to add already added rigidbody"));
    }
#endif

    mRegisteredRigidBodyList.add(body);
}

void Physics::notifyRigidBodyRemoved(RigidBody *body) {
    mRegisteredRigidBodyList.remove(body);
}

void Physics::notifyColliderCreated(Collider *collider) {
#if DEBUG_MODE
    if (mRegisteredColliderList.contains(collider)) {
        LOG(LogPhysics, Warning, TEXT("Try to add already added collider"));
    }
#endif

    mTree.insert(collider);
    mRegisteredColliderList.add(collider);

    if (collider->isTrigger()) {
        mTriggerColliderList.add(collider);
    }
}

void Physics::notifyColliderUpdated(Collider *collider) {
    auto bounds = collider->generateAABB();

    mTree.update(collider);
    if (collider->isTriggerOld() && !collider->isTrigger()) {
        mTriggerColliderList.remove(collider);
    } else if (!collider->isTriggerOld() && collider->isTrigger()) {
        mTriggerColliderList.add(collider);
    }
}

void Physics::notifyColliderRemoved(Collider *collider) {
    auto lastCollider = mRegisteredColliderList.top();

    mTree.remove(collider);
    mRegisteredColliderList.remove(collider);

    if (collider->isTrigger()) {
        mTriggerColliderList.remove(collider);
    }
}

Physics &gPhysics() {
    return Physics::Instance();
}