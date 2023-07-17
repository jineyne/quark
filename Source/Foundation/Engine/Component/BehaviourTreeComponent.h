#pragma once

#include "Prerequisites.h"
#include "Scene/Component.h"
#include "AI/AIBehaviourTree.h"
#include "AI/AIDecoratorNode.h"
#include "AI/AIExecuteNode.h"
#include "AI/AISelectorNode.h"
#include "BehaviourTreeComponent.g.h"

QCLASS()
class DLL_EXPORT BehaviourTreeComponent : public Component {
    GENERATED_BODY()

private:
    AIBehaviourTree *mInternal;
    AIBlackboard *mBlackboard;

public:
    void onCreate() override;
    void onDestroy() override;

    void onFixedUpdate() override;

    AIBehaviourTree *getBehaviourTree() const;
    void setBehaviourTree(AIBehaviourTree *bt);

    AIBlackboard *getBlackboard() const;

    AISelectorNode *addSelectorNode(AICompositeNode *parent = nullptr);
    AISequenceNode *addSequenceNode(AICompositeNode *parent = nullptr);

    template <typename T>
    T *addDecoratorNode(AICompositeNode *parent = nullptr) {
        static_assert(std::is_base_of<AIDecoratorNode, T>::value, "addDecoratorNode type is must derived from AIDecoratorNode");
        if (parent == nullptr) {
            parent = mInternal->getRoot();
        }

        auto node = q_new<T>(nullptr);
        parent->addNode(node);

        return node;
    }

    template <typename T>
    T *addExecuteNode(AICompositeNode *parent = nullptr) {
        static_assert(std::is_base_of<AIExecuteNode, T>::value, "addExecuteNode type is must derived from AIExecuteNode");
        if (parent == nullptr) {
            parent = mInternal->getRoot();
        }

        auto node = q_new<T>();
        parent->addNode(node);

        return node;
    }

    template <typename T>
    T *wrapDecoratorNode(AINode *node) {
        static_assert(std::is_base_of<AIDecoratorNode, T>::value, "wrapDecoratorNode type is must derived from AIDecoratorNode");

        auto parent = dynamic_cast<AICompositeNode *>(node->getParent());
        if (parent == nullptr) {
            return nullptr;
        }

        parent->removeNode(node);

        auto decorator = q_new<T>(node);
        parent->addNode(decorator);

        node->setBehaviourTree(parent->getBehaviourTree());
        node->setParent(decorator);

        return decorator;
    }
};
