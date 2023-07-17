#pragma once

#include "Prerequisites.h"
#include "Resource/Resource.h"
#include "AICompositeNode.h"
#include "AISequenceNode.g.h"

QCLASS()
class DLL_EXPORT AISequenceNode : public AICompositeNode {
    GENERATED_BODY()

public:
    EAIStatus tick() override;
};