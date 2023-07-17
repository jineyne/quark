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
#include "PlayerShipComponent.h"

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

void FighterAIComponent::onFixedUpdate() {
    Component::onFixedUpdate();

    if (mTarget) {
        if (mTarget->getTransform()->getPosition().distance(getTransform()->getPosition()) <= 100) {
            getBehaviourTree()->getBlackboard()->setValueAsObject(TEXT("Target"), mTarget);
        } else {
            getBehaviourTree()->getBlackboard()->setValueAsObject(TEXT("Target"), nullptr);
        }
    }
}


void FighterAIComponent::onDetectCollisionEnter(Collider *collider) {
    auto actor = collider->getTransform()->getOwner();

    // is ship?
    auto ai = actor->getComponent<PlayerShipComponent>();
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

    ai->Destroyed.bindDynamic(FighterAIComponent::onTargetDestroyed);

    getBehaviourTree()->getBlackboard()->setValueAsObject(TEXT("Target"), mTarget);
}

void FighterAIComponent::onTargetDestroyed() {
    // auto ai = (PlayerShipComponent *) mTargetAI;
    // ai->Destroyed.unbind(&FighterAIComponent::onTargetDestroyed, this);

    mTarget = nullptr;
    mTargetAI = nullptr;

    getBehaviourTree()->getBlackboard()->setValueAsObject(TEXT("Target"), nullptr);
}

void FighterAIComponent::setupAI() {
    ShipAIComponent::setupAI();

    auto selector = getBehaviourTree()->addSelectorNode();
    {
        // setup to attack target
        auto sequence = getBehaviourTree()->addSequenceNode(selector);
        auto isTargetSet = getBehaviourTree()->wrapDecoratorNode<AIBlackboardDecoratorNode>(sequence);
        isTargetSet->setBlackboardKey(TEXT("Target"));
        isTargetSet->setKeyQuery(EBlackboardDecoratorKeyQuery::IsSet);

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
        executor->setAcceptanceRadius(1);

        auto decorator = getBehaviourTree()->wrapDecoratorNode<AIBlackboardDecoratorNode>(executor);
        decorator->setBlackboardKey(TEXT("Target"));
        decorator->setKeyQuery(EBlackboardDecoratorKeyQuery::IsNotSet);
    }
}
