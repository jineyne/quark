#include "Physics.h"
#include "Scene/Transform.h"
#include "Misc/Time.h"

DEFINE_LOG_CATEGORY(LogPhysics)

void Physics::onStartUp() {
    mOctree = q_new<Octree<Collider *>>(Vector3(1000, 1000, 1000));
}

void Physics::onShutDown() {
    q_delete(mOctree);
}

void Physics::fixedUpdate(float st) {
    mOctree->recombine();

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

    auto collisions = mOctree->resolveCollisions(st);
    for (auto collision : collisions) {
        auto points = collision.key->testCollision(collision.key->getTransform(), collision.value, collision.value->getTransform());

        // not collision
        if (!points.hasCollision) {
            continue;
        }

        if (collision.key->isTrigger()) {
            collision.key->CollisionEnter(collision.value);
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


    auto bounds = collider->generateAABB();
    if (mOctree->insert(collider, bounds)) {
        collider->setRegisteredBounds(bounds);
        collider->setPhysicsId(mRegisteredColliderList.length());

        mRegisteredColliderList.add(collider);
    }
}

void Physics::notifyColliderUpdated(Collider *collider) {
    auto physicsId = collider->getPhysicsId();

    auto bounds = collider->generateAABB();
    if (mOctree->update(collider, collider->getRegisteredBounds(), bounds)) {
        collider->setRegisteredBounds(bounds);
    }
}

void Physics::notifyColliderRemoved(Collider *collider) {
    auto colliderId = collider->getPhysicsId();

    auto lastCollider = mRegisteredColliderList.top();
    auto lastColliderId = lastCollider->getPhysicsId();

    if (colliderId != lastColliderId) {
        std::swap(mRegisteredColliderList[colliderId], mRegisteredColliderList[lastColliderId]);
        lastCollider->setPhysicsId(colliderId);
    }

    mOctree->remove(collider, collider->getRegisteredBounds());
    mRegisteredColliderList.remove(collider);
}

Physics &gPhysics() {
    return Physics::Instance();
}