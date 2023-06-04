#include "AIBehaviourTree.h"

#include "AISequenceNode.h"

AIBehaviourTree::AIBehaviourTree() {
    mRoot = q_new<AISequenceNode>();
    mRoot->setBehaviourTree(this);
}

AIBehaviourTree::~AIBehaviourTree() {
    q_delete(mRoot);
}

EAIStatus AIBehaviourTree::tick() {
    return mRoot->tick();
}

AICompositeNode *AIBehaviourTree::getRoot() const {
    return mRoot;
}

AIBlackboard *AIBehaviourTree::getBlackboard() const {
    return mBlackboard;
}

void AIBehaviourTree::setBlackboard(AIBlackboard *blackboard) {
    mBlackboard = blackboard;
}

Actor *AIBehaviourTree::getActor() const {
    return mActor;
}

void AIBehaviourTree::setActor(Actor *actor) {
    mActor = actor;
}
