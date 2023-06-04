#include "FighterAIComponent.h"
#include "Scene/Actor.h"
#include "Misc/Time.h"
#include "Component/SphereColliderComponent.h"
#include "Component/RigidBodyComponent.h"
#include "AI/AIBehaviourTree.h"
#include "Component/BehaviourTreeComponent.h"
#include "AI/Execute/AIMoveRandomPositionExecuteNode.h"
#include "AI/Decorator/AIBlackboardDecoratorNode.h"
#include "AI/AIAttackTargetExecuteNode.h"
#include "AI/Decorator/AICooldownDecoratorNode.h"
#include "AI/AILookTargetExecuteNode.h"

void FighterAIComponent::onCreate() {
    Super::onCreate();

    // detect range
    auto collider = getOwner()->addComponent<SphereColliderComponent>();
    collider->setRadius(mDetectRange);

    collider->CollisionEnter.bindDynamic(FighterAIComponent::onDetectCollisionEnter);
}

void FighterAIComponent::onStart() {
    Component::onStart();

    mRigidBody = getOwner()->getComponent<RigidBodyComponent>();
}

void FighterAIComponent::onDetectCollisionEnter(Collider *collider) {
    auto actor = collider->getTransform()->getOwner();

    // is ship?
    auto ai = actor->getComponent<ShipAIComponent>();
    if (ai == nullptr) {
        return;
    }

    // is in detect range?
    if (getTransform()->getPosition().distance(actor->getTransform()->getPosition()) > mDetectRange) {
        return;
    }

    // is team?
    if (ai->getTeam() == getTeam()) {
        return;
    }

    mTarget = actor;
    mTargetAI = ai;

    getBehaviourTree()->getBlackboard()->setValueAsObject(TEXT("Target"), mTarget);
}

void FighterAIComponent::setupAI() {
    ShipAIComponent::setupAI();

    auto selector = getBehaviourTree()->addSelectorNode();
    {
        // setup to attack target
        auto isTargetSet = getBehaviourTree()->addDecoratorNode<AIBlackboardDecoratorNode>();
        isTargetSet->setBlackboardKey(TEXT("Target"));
        isTargetSet->setKeyQuery(AIBlackboardDecoratorNode::EBlackboardDecoratorKeyQuery::IsSet);

        auto sequence = getBehaviourTree()->addSequenceNode(isTargetSet);

        // look enemy
        auto lookAtTarget = getBehaviourTree()->addExecuteNode<AILookTargetExecuteNode>(sequence);

        // attack enemy with cooldown
        auto attackTarget = getBehaviourTree()->addExecuteNode<AIAttackTargetExecuteNode>(sequence);
        auto cooldown = getBehaviourTree()->wrapDecoratorNode<AICooldownDecoratorNode>(attackTarget);
        cooldown->setCooldown(1);
    }
    {
        // setup to not find target
        auto executor = getBehaviourTree()->addExecuteNode<AIMoveRandomPositionExecuteNode>(selector);
        executor->setSpeed(10);

        auto decorator = getBehaviourTree()->wrapDecoratorNode<AIBlackboardDecoratorNode>(executor);
        decorator->setBlackboardKey(TEXT("Target"));
        decorator->setKeyQuery(AIBlackboardDecoratorNode::EBlackboardDecoratorKeyQuery::IsNotSet);
    }
}

