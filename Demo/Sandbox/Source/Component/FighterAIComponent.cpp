#include "FighterAIComponent.h"
#include "Scene/Actor.h"
#include "Misc/Time.h"
#include "AI/AIBehaviourTree.h"
#include "Component/BehaviourTreeComponent.h"
#include "AI/Execute/AIMoveRandomPositionExecuteNode.h"
#include "AI/Decorator/AIBlackboardDecoratorNode.h"
#include "AI/AIAttackTargetExecuteNode.h"
#include "AI/Decorator/AICooldownDecoratorNode.h"
#include "AI/AILookTargetExecuteNode.h"
#include "PlayerShipComponent.h"

void FighterAIComponent::onCreate() {
    ShipAIComponent::onCreate();

}

void FighterAIComponent::onStart() {
    ShipAIComponent::onStart();
}

void FighterAIComponent::onFixedUpdate() {
    ShipAIComponent::onFixedUpdate();

    if (mTarget) {
        if (mTarget->getTransform()->getPosition().distance(getTransform()->getPosition()) <= 100) {
            getBehaviourTree()->getBlackboard()->setValueAsObject(TEXT("Target"), mTarget);
        } else {
            getBehaviourTree()->getBlackboard()->setValueAsObject(TEXT("Target"), nullptr);
        }
    }
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
