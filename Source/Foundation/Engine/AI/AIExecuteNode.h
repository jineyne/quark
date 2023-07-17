#pragma once

#include "Prerequisites.h"
#include "Resource/Resource.h"
#include "AINode.h"
#include "AIExecuteNode.g.h"

QCLASS(abstract)
class DLL_EXPORT AIExecuteNode : public AINode {
    GENERATED_BODY();

public:
    EAIStatus tick() override;

    virtual EAIStatus execute() = 0;
};