#include "ShipAIComponent.h"

#include "Scene/Actor.h"
#include "Component/SphereColliderComponent.h"
#include "Component/MeshRendererComponent.h"
#include "Importer/Importer.h"
#include "Source/Utility/SandboxResourceUtil.h"
#include "BulletComponent.h"

void ShipAIComponent::onCreate() {
    Super::onCreate();

    auto collider = getOwner()->addComponent<SphereColliderComponent>();
    collider->setRadius(8);

    collider->CollisionEnter.bindDynamic(ShipAIComponent::onShipCollisionEnter);
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
}

void ShipAIComponent::onShipCollisionEnter(Collider *other) {
    auto otherActor = other->getTransform()->getOwner();
    if (otherActor->getTags() != TEXT("Bullet")) {
        return;
    }

    auto bullet = otherActor->getComponent<BulletComponent>();
    if (bullet->getTeam() == mTeam) {
        return;
    }

    mHealth -= bullet->getDamage();
    if (mHealth <= 0) {
        getOwner()->destroy();
    }

    otherActor->destroy();
}
