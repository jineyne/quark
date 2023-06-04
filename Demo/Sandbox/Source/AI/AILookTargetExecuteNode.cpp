#include "AILookTargetExecuteNode.h"

#include "AI/AIBehaviourTree.h"
#include "AI/AIBlackboard.h"

#include "Scene/Actor.h"
#include "Scene/Transform.h"

EAIStatus AILookTargetExecuteNode::execute() {
    auto target = (Actor *) getBlackboard()->getValueAsObject(TEXT("Target"));
    auto team = getBlackboard()->getValueAsInt(TEXT("Team"));
    if (target == nullptr) {
        return EAIStatus::Failure;
    }

    auto actor = getBehaviourTree()->getActor();
    auto transform = actor->getTransform();

    transform->lookAt(target->getTransform()->getPosition(), target->getTransform()->getUp());

    return EAIStatus::Success;
}
