#include "SphereCollider2DComponent.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"

void SphereCollider2DComponent::onCreate() {
    auto scene = getOwner()->getScene();
    auto physicsWorld = scene->getPhysicsScene();

    mInternal = physicsWorld->createSphereCollider2D(mRadius, getTransform());

    Collider2DComponent::onCreate();
}

void SphereCollider2DComponent::onDestroy() {
    Component::onDestroy();

    q_delete(mInternal);
}

void SphereCollider2DComponent::onFixedUpdate() {
    Component::onFixedUpdate();

    if (!isActive()) {
        return;
    }

    auto transform = getTransform();

    auto position = transform->getPosition();
    auto pos2d = Vector2(position.x, position.y);
    auto physicsPosition = mInternal->getPosition();

    if (pos2d != physicsPosition) {
        transform->setPosition({ physicsPosition.x, physicsPosition.y, position.z });
        transform->setDirty(false);
    }

    auto rotation = transform->getRotation();
    auto physicsRotation = mInternal->getRotation();

    if (rotation != physicsRotation) {
        transform->setRotation(physicsRotation);
        transform->setDirty(false);
    }
}

void SphereCollider2DComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    Component::onTransformChanged(flags);

    if ((flags & ETransformChangedFlags::Transform) == ETransformChangedFlags::Transform) {
        auto transform = getTransform();

        auto position = transform->getPosition();
        auto pos2d = Vector2(position.x, position.y);

        auto rotation = transform->getRotation();

        mInternal->updateTransform(pos2d, rotation);
    }
}

float SphereCollider2DComponent::getRadius() const {
    return mRadius;
}

void SphereCollider2DComponent::setRadius(float radius) {
    if (mRadius == radius) {
        return;
    }

    mRadius = radius;
    if (isActive()) {
        _internal()->setRadius(mRadius);
    }
}

SphereCollider2D *SphereCollider2DComponent::_internal() const {
    return (SphereCollider2D *) mInternal;
}
