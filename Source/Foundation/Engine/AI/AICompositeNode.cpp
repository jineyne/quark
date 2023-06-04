#include "AICompositeNode.h"

#include "AIBehaviourTree.h"

AICompositeNode::~AICompositeNode() {
    for (auto child : mChildren) {
        q_delete(child);
    }
}

void AICompositeNode::addNode(AINode *node) {
    node->setBehaviourTree(getBehaviourTree());
    node->setParent(this);

    mChildren.add(node);
}

void AICompositeNode::removeNode(AINode *node) {
    node->setBehaviourTree(nullptr);
    node->setParent(nullptr);

    mChildren.remove(node);
}
