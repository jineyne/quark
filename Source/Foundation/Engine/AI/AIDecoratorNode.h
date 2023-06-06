#pragma once

#include "Prerequisites.h"
#include "Resource/Resource.h"
#include "AISequenceNode.h"

class DLL_EXPORT AIDecoratorNode : public AISequenceNode {
protected:
    AINode *mNode;

public:
    AIDecoratorNode(AINode *node);
    virtual ~AIDecoratorNode();

public:
    virtual bool conditionCheck() const;
    EAIStatus tick() override;

    AINode *getNode() const;
    void setNode(AINode *node);
};
