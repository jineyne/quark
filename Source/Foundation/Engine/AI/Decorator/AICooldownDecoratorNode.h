#pragma once

#include "Prerequisites.h"
#include "AI/AIDecoratorNode.h"
#include "AI/AIBlackboard.h"

class DLL_EXPORT AICooldownDecoratorNode : public AIDecoratorNode {
private:
    float mCooldown = 1;
    float mInternalCooldown = 0;

public:
    AICooldownDecoratorNode(AINode *node);

public:
    bool conditionCheck() const override;
    EAIStatus tick() override;

    float getCooldown() const;
    void setCooldown(float cooldown);
};
