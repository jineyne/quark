//
// Created by jiney on 2023-10-15.
//

#include "Collider2DComponent.h"

void Collider2DComponent::onCreate() {
    Component::onCreate();

    mInternal->CollisionEnter.bindDynamic(Collider2DComponent::OnCollision2DEnter);
    mInternal->CollisionExit.bindDynamic(Collider2DComponent::OnCollision2DExit);
}

void Collider2DComponent::onUpdate() {
    Component::onUpdate();

    // :D
    for (auto collider : mEnteredCollider) {
        CollisionStay(collider);
    }
}

bool Collider2DComponent::isIsTrigger() const {
    return bIsTrigger;
}

const Vector2 &Collider2DComponent::getOffset() const {
    return mOffset;
}

void Collider2DComponent::setOffset(const Vector2 &offset) {
    if (mOffset == offset) {
        return;
    }

    mOffset = offset;
    if (isActive()) {
        mInternal->setOffset(mOffset);
    }
}

void Collider2DComponent::setIsTrigger(bool isTrigger) {
    if (bIsTrigger == isTrigger) {
        return;
    }

    bIsTrigger = isTrigger;
    if (isActive()) {
        mInternal->setIsTrigger(bIsTrigger);
    }
}

void Collider2DComponent::setBodyType(EPhysicsBodyType type) {
    if (mBodyType == type) {
        return;
    }

    mBodyType = type;
    mInternal->setPhysicsBodyType(type);
}

void Collider2DComponent::OnCollision2DEnter(Collider2D *collider) {
    CollisionEnter(collider);

    mEnteredCollider.add(collider);
}

void Collider2DComponent::OnCollision2DExit(Collider2D *collider) {
    CollisionExit(collider);

    mEnteredCollider.remove(collider);
}

void Collider2DComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    Component::onTransformChanged(flags);

    if ((flags & ETransformChangedFlags::Transform) == ETransformChangedFlags::Transform) {
        for (auto collider : mEnteredCollider) {
            collider->setAwake(true);
        }
    }
}

