#include "FighterAIComponent.h"
#include "Scene/Actor.h"
#include "Scene/Transform.h"
#include "Misc/Time.h"
#include "BulletComponent.h"
#include "Component/MeshRendererComponent.h"
#include "Importer/Importer.h"
#include "Source/Utility/SandboxResourceUtil.h"

void FighterAIComponent::setTeam(int team) {
    mTeam = team;
}

void FighterAIComponent::fire() {
    auto bulletActor = Actor::New(TEXT("Bullet"));
    bulletActor->getTransform()->setPosition(getTransform()->getPosition());
    bulletActor->getTransform()->setRotation(getTransform()->getRotation());
    bulletActor->getTransform()->setScale(Vector3(0.01, 0.01, 0.01));

    bulletActor->addComponent<BulletComponent>();

    auto renderer = bulletActor->addComponent<MeshRendererComponent>();
    renderer->setMesh(gImporter().import<Mesh>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Model\\SM_Primitive_Cube_02.fbx")));
    renderer->setMaterial(SandboxResourceUtil::GetBulletMaterial());
}

void FighterAIComponent::onCreate() {
    Component::onCreate();

    mPlayer = Actor::Find(TEXT("Player"));
}

void FighterAIComponent::onUpdate() {
    ShipComponent::onUpdate();

    mBulletInterval -= gTime().getDeltaTime();

    if (Vector3::Distance(mPlayer->getTransform()->getPosition(), getTransform()->getPosition()) <= 100) {
        getTransform()->lookAt(mPlayer->getTransform()->getPosition(), mPlayer->getTransform()->getUp());

        auto euler = getTransform()->getRotation().toEulerAngles();
        // LOG(LogTemp, Debug, TEXT("Rot: %lf, %lf, %lf"), euler.x, euler.y, euler.z);

        if (mBulletInterval <= 0) {
            mBulletInterval = 1;

            fire();
        }
    }
}
