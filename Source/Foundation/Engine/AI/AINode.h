#pragma once

#include "Prerequisites.h"
#include "Resource/Resource.h"
#include "AITypes.h"

class DLL_EXPORT AINode {
private:
    class AIBehaviourTree *mBehaviourTree = nullptr;
    AINode *mParent = nullptr;

public:
    virtual ~AINode() = default;

public:
    virtual EAIStatus tick() = 0;

    void setBehaviourTree(AIBehaviourTree *behaviourTree);
    AIBehaviourTree *getBehaviourTree() const;

    class AIBlackboard *getBlackboard() const;

    AINode *getParent() const;
    void setParent(AINode *parent);
};