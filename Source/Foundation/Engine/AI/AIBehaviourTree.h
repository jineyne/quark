#pragma once

#include "Prerequisites.h"
#include "Resource/Resource.h"
#include "AINode.h"
#include "AIBlackboard.h"
#include "AIBehaviourTree.g.h"

QCLASS()
class DLL_EXPORT AIBehaviourTree : public Resource {
    GENERATED_BODY()

private:
    class AICompositeNode *mRoot;
    AIBlackboard *mBlackboard;
    Actor *mActor;

public:
    AIBehaviourTree();
    ~AIBehaviourTree();

public:
    EAIStatus tick();

    AICompositeNode *getRoot() const;

    AIBlackboard *getBlackboard() const;
    void setBlackboard(AIBlackboard *blackboard);

    Actor *getActor() const;
    void setActor(Actor *actor);
};