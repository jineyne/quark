//
// Created by jiney on 2023-10-15.
//

#include "Collider2DComponent.h"

void Collider2DComponent::onCreate() {
    Component::onCreate();

    mInternal->CollisionEnter.bindLambda([&](Collider2D *collider2D) { CollisionEnter(collider2D); });
    mInternal->CollisionExit.bindLambda([&](Collider2D *collider2D) { CollisionExit(collider2D); });
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

