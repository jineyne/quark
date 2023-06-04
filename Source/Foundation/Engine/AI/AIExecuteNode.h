#pragma once

#include "Prerequisites.h"
#include "Resource/Resource.h"
#include "AINode.h"

class DLL_EXPORT AIExecuteNode : public AINode {
public:
    EAIStatus tick() override;

    virtual EAIStatus execute() = 0;
};