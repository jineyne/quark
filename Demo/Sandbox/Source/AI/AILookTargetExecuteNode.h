#pragma once

#include "Sandbox.h"
#include "AI/AIExecuteNode.h"

class AILookTargetExecuteNode : public AIExecuteNode {
public:
    EAIStatus execute() override;
};