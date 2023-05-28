#include "Physics.h"
#include "Scene/Transform.h"
#include "Misc/Time.h"

DEFINE_LOG_CATEGORY(LogPhysics)

void Physics::update() {
    auto dt = gTime().getDeltaTime();

    for (auto *body : mRegisteredRigidBodyList) {
        Transform *transform = body->getTransform();

        Vector3 velocity = body->getVelocity();
        Vector3 force = body->getForce();
        Vector3 position = transform->getPosition();

        force += mGravity * body->getMass();
        velocity += force / body->getMass() * dt;
        position += velocity * dt;

        body->setForce(Vector3::ZeroVector);
        body->setVelocity(velocity);
        transform->setPosition(position);

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


Physics &gPhysics() {
    return Physics::Instance();
}