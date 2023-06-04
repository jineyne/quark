#include "AIRunawayExecuteNode.h"

#include "AI/AIBehaviourTree.h"
#include "AI/AIBlackboard.h"

#include "Scene/Actor.h"
#include "Scene/Transform.h"

#include "Misc/Time.h"

EAIStatus AIRunawayExecuteNode::execute() {
    auto target = (Actor *) getBlackboard()->getValueAsObject(TEXT("Target"));
    auto team = getBlackboard()->getValueAsInt(TEXT("Team"));
    if (target == nullptr) {
        return EAIStatus::Failure;
    }

    auto targetPosition = target->getTransform()->getPosition();

    auto actor = getBehaviourTree()->getActor();
    auto transform = actor->getTransform();
    if (transform->getPosition().distance(targetPosition) < mAcceptanceRadius) {
        transform->setPosition(targetPosition);

        return EAIStatus::Success;
    }

    auto offset = transform->getPosition() - targetPosition;
    offset = offset.normalized() * mSpeed  * gTime().getFixedDeltaTime();
    transform->setPosition(transform->getPosition() + offset);
    transform->lookAt(transform->getPosition() + offset, transform->getUp());

    return EAIStatus::Running;
}

float AIRunawayExecuteNode::getAcceptanceRadius() const {
    return mAcceptanceRadius;
}

void AIRunawayExecuteNode::setAcceptanceRadius(float acceptanceRadius) {
    mAcceptanceRadius = acceptanceRadius;
}

float AIRunawayExecuteNode::getSpeed() const {
    return mSpeed;
}

void AIRunawayExecuteNode::setSpeed(float speed) {
    mSpeed = speed;
}
