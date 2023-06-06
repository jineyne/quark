#include "AICompositeNode.h"

#include "AIBehaviourTree.h"

AICompositeNode::~AICompositeNode() {
    for (auto child : mChildren) {
        q_delete(child);
    }
}

void AICompositeNode::addNode(AINode *node) {
    if (node->getParent()) {
        auto parent = dynamic_cast<AICompositeNode *>(node->getParent());
        if (parent != nullptr) {
            parent->removeNode(node);
        }
    }

    node->setBehaviourTree(getBehaviourTree());
    node->setParent(this);

    mChildren.add(node);
}

void AICompositeNode::removeNode(AINode *node) {
    node->setBehaviourTree(nullptr);
    node->setParent(nullptr);

    mChildren.remove(node);
}
