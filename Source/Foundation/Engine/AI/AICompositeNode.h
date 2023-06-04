#pragma once

#include "Prerequisites.h"
#include "Resource/Resource.h"
#include "AINode.h"

class DLL_EXPORT AICompositeNode : public AINode {
protected:
    TArray<AINode *> mChildren;

public:
    ~AICompositeNode();

public:
    void addNode(AINode *node);
    void removeNode(AINode *node);
};