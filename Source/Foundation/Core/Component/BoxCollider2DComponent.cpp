#include "BoxCollider2DComponent.h"
#include "Physics/Physics.h"
#include "Scene/Actor.h"
#include "Scene/Scene.h"

void BoxCollider2DComponent::onCreate() {
    auto scene = getOwner()->getScene();
    auto physicsWorld = scene->getPhysicsScene();
    mInternal = physicsWorld->createBoxCollider2D(mSize, getTransform());

    Collider2DComponent::onCreate();
}

void BoxCollider2DComponent::onDestroy() {
    Collider2DComponent::onDestroy();

    q_delete(mInternal);
}

void BoxCollider2DComponent::onFixedUpdate() {
    Collider2DComponent::onFixedUpdate();

    if (!isActive()) {
        return;
    }

    auto transform = getTransform();

    auto position = transform->getPosition();
    auto pos2d = Vector2(position.x, position.y);
    auto physicsPosition = mInternal->getPosition();

    if (pos2d != physicsPosition) {
        transform->notifyTransformPositionChanged({physicsPosition.x, physicsPosition.y, position.z});
    }

    auto rotation = transform->getRotation();
    auto physicsRotation = mInternal->getRotation();

    if (rotation != physicsRotation) {
        transform->notifyTransformRotationChanged(physicsRotation);
    }
}

void BoxCollider2DComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    Collider2DComponent::onTransformChanged(flags);

    if ((flags & ETransformChangedFlags::Physics) != ETransformChangedFlags::Physics) {
        auto transform = getTransform();

        auto position = transform->getPosition();
        auto pos2d = Vector2(position.x, position.y);

        auto rotation = transform->getRotation();

        mInternal->updateTransform(pos2d, rotation);
    }
}


BoxCollider2D *BoxCollider2DComponent::_internal() const { return (BoxCollider2D *) mInternal; }

const Size &BoxCollider2DComponent::getSize() const {
    return mSize;
}

void BoxCollider2DComponent::setSize(const Size &size) {
    if (mSize == size) {
        return;
    }

    mSize = size;
    if (isActive()) {
        _internal()->setSize(mSize);
    }
}
