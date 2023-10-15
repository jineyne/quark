#include "ShipAIComponent.h"

#include "Scene/Actor.h"
#include "Scene/Transform.h"
#include "Component/MeshRendererComponent.h"
#include "Importer/Importer.h"
#include "Source/Utility/SandboxResourceUtil.h"
#include "Component/BehaviourTreeComponent.h"
#include "BulletComponent.h"

void ShipAIComponent::onCreate() {
    Component::onCreate();

    setupAI();
}

void ShipAIComponent::fire() {
    auto bulletActor = Actor::New(TEXT("Bullet"));
    bulletActor->setTags(TEXT("Bullet"));
    bulletActor->getTransform()->setPosition(getTransform()->getPosition() + getTransform()->getForward().normalized() * 2);
    bulletActor->getTransform()->setRotation(getTransform()->getRotation());
    bulletActor->getTransform()->setScale(Vector3(0.01, 0.01, 0.01));

    auto bullet = bulletActor->addComponent<BulletComponent>();
    bullet->setTeam(getTeam());

    auto renderer = bulletActor->addComponent<MeshRendererComponent>();
    renderer->setMesh(gImporter().import<Mesh>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Model\\SM_Primitive_Cube_02.fbx")));
    renderer->setMaterial(SandboxResourceUtil::GetBulletMaterial());
}

void ShipAIComponent::assignTo(ShipAIComponent *parent) {
    mParentAI = parent;
    parent->addChild(this);
}

void ShipAIComponent::dissociate() {
    mParentAI->removeChild(this);
    mParentAI = nullptr;
}

float ShipAIComponent::getHealth() const {
    return mHealth;
}

void ShipAIComponent::setHealth(float health) {
    mHealth = health;
}

BehaviourTreeComponent *ShipAIComponent::getBehaviourTree() const {
    return mBehaviourTree;
}

void ShipAIComponent::setupAI() {
    mBehaviourTree = getOwner()->addComponent<BehaviourTreeComponent>();
    auto bb = mBehaviourTree->getBlackboard();
    bb->setValueAsInt(TEXT("Team"), getTeam());
}

void ShipAIComponent::addChild(ShipAIComponent *child) {
    if (mChildAIList.contains(child)) {
        return;
    }

    mChildAIList.add(child);
}

void ShipAIComponent::removeChild(ShipAIComponent *child) {
    if (!mChildAIList.contains(child)) {
        return;
    }

    mChildAIList.remove(child);
}

int ShipAIComponent::getTeam() const {
    return mTeam;
}

void ShipAIComponent::setTeam(int team) {
    mTeam = team;

    mBehaviourTree->getBlackboard()->setValueAsInt(TEXT("Team"), getTeam());
}
