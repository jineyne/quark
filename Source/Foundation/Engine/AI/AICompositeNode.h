#pragma once

#include "Prerequisites.h"
#include "Resource/Resource.h"
#include "AINode.h"
#include "AICompositeNode.g.h"

QCLASS(abstract)
class DLL_EXPORT AICompositeNode : public AINode {
    GENERATED_BODY();

protected:
    QPROPERTY()
    TArray<AINode *> mChildren;

public:
    ~AICompositeNode();

public:
    void addNode(AINode *node);
    void removeNode(AINode *node);
};