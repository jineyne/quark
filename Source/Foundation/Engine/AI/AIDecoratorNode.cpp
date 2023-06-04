#include "AIDecoratorNode.h"

AIDecoratorNode::AIDecoratorNode(AINode *node) : mNode(node) {
    node->setBehaviourTree(getBehaviourTree());
    node->setParent(this);
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
