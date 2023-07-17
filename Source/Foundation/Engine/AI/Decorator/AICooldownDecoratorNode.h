#pragma once

#include "Prerequisites.h"
#include "AI/AIDecoratorNode.h"
#include "AI/AIBlackboard.h"
#include "AICooldownDecoratorNode.g.h"

QCLASS()
class DLL_EXPORT AICooldownDecoratorNode : public AIDecoratorNode {
    GENERATED_BODY();

private:
    QPROPERTY()
    float mCooldown = 1;

    QPROPERTY()
    float mInternalCooldown = 0;

private:
    AICooldownDecoratorNode() = default;

public:
    AICooldownDecoratorNode(AINode *node);

public:
    bool conditionCheck() const override;
    EAIStatus tick() override;

    float getCooldown() const;
    void setCooldown(float cooldown);
};
