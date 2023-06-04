#include "AINode.h"

#include "AIBehaviourTree.h"
#include "AIBlackboard.h"

EAIStatus AINode::tick() {
    return EAIStatus::Failure;
}

void AINode::setBehaviourTree(AIBehaviourTree *behaviourTree) {
    mBehaviourTree = behaviourTree;
}

AIBehaviourTree *AINode::getBehaviourTree() const {
    return mBehaviourTree;
}

class AIBlackboard *AINode::getBlackboard() const {
    if (mBehaviourTree == nullptr) {
        return nullptr;
    }

    return mBehaviourTree->getBlackboard();
}

AINode *AINode::getParent() const {
    return mParent;
}

void AINode::setParent(AINode *parent) {
    mParent = parent;
}
