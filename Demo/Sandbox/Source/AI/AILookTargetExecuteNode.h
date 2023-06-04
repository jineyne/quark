#pragma once

#include "Prerequisites.h"
#include "AI/AIExecuteNode.h"

class AILookTargetExecuteNode : public AIExecuteNode {
public:
    EAIStatus execute() override;
};