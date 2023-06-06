#include "AIDecoratorNode.h"

AIDecoratorNode::AIDecoratorNode(AINode *node) : mNode(node) {
    if (node != nullptr) {
        node->setBehaviourTree(getBehaviourTree());
        node->setParent(this);
    }
}

AIDecoratorNode::~AIDecoratorNode() {
    q_delete(mNode);
}

bool AIDecoratorNode::conditionCheck() const {
    return true;
}

EAIStatus AIDecoratorNode::tick() {
    if (mNode && conditionCheck()) {
        return mNode->tick();
    } else {
        return EAIStatus::Failure;
    }
}

AINode *AIDecoratorNode::getNode() const {
    return mNode;
}

void AIDecoratorNode::setNode(AINode *node) {
    mNode = node;

    node->setBehaviourTree(getBehaviourTree());
    node->setParent(this);
}
