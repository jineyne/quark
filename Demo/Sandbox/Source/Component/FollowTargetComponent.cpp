#include "FollowTargetComponent.h"

#include "Scene/Actor.h"
#include "Scene/Transform.h"

void FollowTargetComponent::onFixedUpdate() {
    Component::onFixedUpdate();

    if (mTarget != nullptr) {
        auto transform = mTarget->getTransform();
        auto position = /*transform->getPosition() +*/ mOffset;

        getTransform()->setPosition(position);
    }
}

Actor *FollowTargetComponent::getTarget() const {
    return mTarget;
}

void FollowTargetComponent::setTarget(Actor *target) {
    mTarget = target;
}

const Vector3 &FollowTargetComponent::getOffset() const {
    return mOffset;
}

void FollowTargetComponent::setOffset(const Vector3 &offset) {
    mOffset = offset;
}
