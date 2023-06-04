#include "AIAttackTargetExecuteNode.h"

#include "AI/AIBehaviourTree.h"
#include "AI/AIBlackboard.h"

#include "Component/MeshRendererComponent.h"
#include "Importer/Importer.h"
#include "Scene/Actor.h"
#include "Scene/Transform.h"

#include "Component/BulletComponent.h"
#include "Utility/SandboxResourceUtil.h"

EAIStatus AIAttackTargetExecuteNode::execute() {
    auto target = (Actor *) getBlackboard()->getValueAsObject(TEXT("Target"));
    auto team = getBlackboard()->getValueAsInt(TEXT("Team"));
    if (target == nullptr) {
        return EAIStatus::Failure;
    }

    auto actor = getBehaviourTree()->getActor();
    auto transform = actor->getTransform();

    auto bulletActor = Actor::New(TEXT("Bullet"));
    bulletActor->setTags(TEXT("Bullet"));
    bulletActor->getTransform()->setPosition(transform->getPosition() + transform->getForward().normalized() * 2);
    bulletActor->getTransform()->setRotation(transform->getRotation());
    bulletActor->getTransform()->setScale(Vector3(0.01, 0.01, 0.01));

    auto bullet = bulletActor->addComponent<BulletComponent>();
    bullet->setTeam(team);

    auto renderer = bulletActor->addComponent<MeshRendererComponent>();
    renderer->setMesh(gImporter().import<Mesh>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Model\\SM_Primitive_Cube_02.fbx")));
    renderer->setMaterial(SandboxResourceUtil::GetBulletMaterial());

    return EAIStatus::Success;
}
