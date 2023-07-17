#pragma once

#include "Prerequisites.h"
#include "AINode.h"
#include "AISequenceNode.h"
#include "AIDecoratorNode.g.h"

QCLASS()
class DLL_EXPORT AIDecoratorNode : public AISequenceNode {
    GENERATED_BODY();

protected:
    QPROPERTY()
    AINode *mNode = nullptr;

public:
    AIDecoratorNode() = default;
    AIDecoratorNode(AINode *node);
    virtual ~AIDecoratorNode();

public:
    virtual bool conditionCheck() const;
    EAIStatus tick() override;

    AINode *getNode() const;
    void setNode(AINode *node);
};
