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
    Component::onDestroy();

    q_delete(mInternal);
}

void BoxCollider2DComponent::onFixedUpdate() {
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

void BoxCollider2DComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    Component::onTransformChanged(flags);

    if ((flags & ETransformChangedFlags::Transform) == ETransformChangedFlags::Transform) {
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
