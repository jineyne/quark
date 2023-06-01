#include "BattleFleetAIComponent.h"
#include "Component/MeshRendererComponent.h"

#include "Scene/Actor.h"
#include "Scene/Transform.h"
#include "Source/Utility/SandboxResourceUtil.h"
#include "FighterAIComponent.h"

void BattleFleetAIComponent::spawnFighter() {
    Actor *actor = Actor::New(TEXT("Fighter"));
    actor->getTransform()->setScale(Vector3(0.01f, 0.01f, 0.01f));

    auto renderer = actor->addComponent<MeshRendererComponent>();
    renderer->setMesh(SandboxResourceUtil::GetSparrowMesh());
    switch (getTeam()) {
        case 0:
            // ONLY FOR PLAYER
            // renderer->setMaterial(gRedMaterial);
            break;

        case 1:
            renderer->setMaterial(SandboxResourceUtil::GetBlueShipMaterial());
            break;

        case 2:
            renderer->setMaterial(SandboxResourceUtil::GetGreenShipMaterial());
            break;
    }

    auto ai = actor->addComponent<FighterAIComponent>();
    ai->setTeam(getTeam());
}
