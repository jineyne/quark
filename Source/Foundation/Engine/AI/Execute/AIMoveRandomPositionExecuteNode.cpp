#include "AIMoveRandomPositionExecuteNode.h"

#include "AI/AIBehaviourTree.h"
#include "Scene/Actor.h"
#include "Scene/Transform.h"
#include "Misc/Time.h"

EAIStatus AIMoveRandomPositionExecuteNode::execute() {
    auto actor = getBehaviourTree()->getActor();
    auto transform = actor->getTransform();
    if (transform->getPosition().distance(mDestination) < mAcceptanceRadius) {
        transform->setPosition(mDestination);

        return EAIStatus::Success;
    }

    auto offset = mDestination - transform->getPosition();
    offset = offset.normalized() * mSpeed  * gTime().getFixedDeltaTime();
    transform->setPosition(transform->getPosition() + offset);
    transform->lookAt(mDestination, transform->getUp());

    return EAIStatus::Running;
}

const Vector3 &AIMoveRandomPositionExecuteNode::getDestination() const {
    return mDestination;
}

void AIMoveRandomPositionExecuteNode::setDestination(const Vector3 &destination) {
    mDestination = destination;
}

float AIMoveRandomPositionExecuteNode::getAcceptanceRadius() const {
    return mAcceptanceRadius;
}

void AIMoveRandomPositionExecuteNode::setAcceptanceRadius(float acceptanceRadius) {
    mAcceptanceRadius = acceptanceRadius;
}

float AIMoveRandomPositionExecuteNode::getSpeed() const {
    return mSpeed;
}

void AIMoveRandomPositionExecuteNode::setSpeed(float speed) {
    mSpeed = speed;
}
