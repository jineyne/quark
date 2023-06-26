#pragma once

#include "Sandbox.h"
#include "AI/AIExecuteNode.h"

class AIAttackTargetExecuteNode : public AIExecuteNode {
public:
    EAIStatus execute() override;
};