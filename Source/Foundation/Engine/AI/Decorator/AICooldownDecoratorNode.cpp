#include "AICooldownDecoratorNode.h"
#include "Misc/Time.h"

AICooldownDecoratorNode::AICooldownDecoratorNode(AINode *node) : AIDecoratorNode(node) {}

bool AICooldownDecoratorNode::conditionCheck() const {
    return mInternalCooldown <= 0;
}

EAIStatus AICooldownDecoratorNode::tick() {
    mInternalCooldown -= gTime().getFixedDeltaTime();

    auto status = AIDecoratorNode::tick();

    if (status != EAIStatus::Failure) {
        mInternalCooldown = mCooldown;
    }

    return status;
}

float AICooldownDecoratorNode::getCooldown() const {
    return mCooldown;
}

void AICooldownDecoratorNode::setCooldown(float cooldown) {
    mCooldown = cooldown;
}
