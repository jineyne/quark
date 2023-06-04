#include "BehaviourTreeComponent.h"

#include "AI/AICompositeNode.h"
#include "AI/AIDecoratorNode.h"
#include "AI/AIExecuteNode.h"

void BehaviourTreeComponent::onCreate() {
    Component::onCreate();

    mInternal = q_new<AIBehaviourTree>();
    mBlackboard = q_new<AIBlackboard>();

    mInternal->setBlackboard(mBlackboard);
    mInternal->setActor(getOwner());
}

void BehaviourTreeComponent::onDestroy() {
    Component::onDestroy();

    q_delete(mBlackboard);
    q_delete(mInternal);
}

void BehaviourTreeComponent::onFixedUpdate() {
    mInternal->tick();
}

AIBehaviourTree *BehaviourTreeComponent::getBehaviourTree() const {
    return mInternal;
}

void BehaviourTreeComponent::setBehaviourTree(AIBehaviourTree *bt) {
    mInternal = bt;
}

AIBlackboard *BehaviourTreeComponent::getBlackboard() const {
    return mBlackboard;
}

AISelectorNode *BehaviourTreeComponent::addSelectorNode(AICompositeNode *parent) {
    if (parent == nullptr) {
        parent = mInternal->getRoot();
    }

    auto node = q_new<AISelectorNode>();
    parent->addNode(node);

    return node;
}

AISequenceNode *BehaviourTreeComponent::addSequenceNode(AICompositeNode *parent) {
    if (parent == nullptr) {
        parent = mInternal->getRoot();
    }

    auto node = q_new<AISequenceNode>();
    parent->addNode(node);

    return node;
}

