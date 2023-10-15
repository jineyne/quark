#include "Application.h"

#include "RenderAPI/GpuParamBlockBuffer.h"
#include "RenderAPI/VertexDataDesc.h"
#include "Resource/Resources.h"
#include "FileSystem/FileSystem.h"
#include "Math/Radian.h"
#include "Math/Vector3.h"
#include "RenderAPI/SamplerState.h"
#include "Input/InputType.h"
#include "Component/CameraComponent.h"
#include "Component/LightComponent.h"
#include "Component/MeshRendererComponent.h"
#include "Scene/Actor.h"
#include "Manager/SceneManager.h"
#include "Component/FighterAIComponent.h"
#include "Utility/SandboxResourceUtil.h"
#include "Source/Component/PlayerInputComponent.h"
#include "Component/FollowTargetComponent.h"
#include "Component/PlayerShipComponent.h"
#include "Serialization/BinaryArchive.h"
#include "Serialization/Formatter/YamlArchiveFormatter.h"

Actor *spawnFighter(int team) {
    Actor *actor = Actor::New(TEXT("Fighter"));
    actor->getTransform()->setScale(Vector3(0.01f, 0.01f, 0.01f));

    auto renderer = actor->addComponent<MeshRendererComponent>();
    renderer->setMesh(SandboxResourceUtil::GetSparrowMesh());
    switch (team) {
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
    ai->setTeam(team);

    return actor;
}

void setupDemoScene() {
    Actor *player = Actor::New(TEXT("Player"));
    player->getTransform()->setScale(Vector3(0.01f, 0.01f, 0.01f));

    auto ship = player->addComponent<PlayerShipComponent>();
    ship->setTeam(0);

    player->addComponent<PlayerInputComponent>();

    // Player

    auto renderer = player->addComponent<MeshRendererComponent>();
    renderer->setMaterial(SandboxResourceUtil::GetRedShipMaterial());
    renderer->setMesh(SandboxResourceUtil::GetSparrowMesh());

    Actor *planeActor = spawnFighter(1);
    planeActor->getTransform()->setPosition(Vector3(0, 0, 130));
    planeActor->getTransform()->setRotation(FQuaternion(0.841471016, 0, 0.540302277, 0));

    Actor *plane2Actor = spawnFighter(1);
    plane2Actor->getTransform()->setPosition(Vector3(0, 0, -130));

    Actor *lightActor = Actor::New(TEXT("Light"));
    auto light = lightActor->addComponent<LightComponent>();
    light->setIntensity(1);
    light->setType(ELightType::Directional);
    light->setColor(Color::White);
    light->getTransform()->setRotation(FQuaternion(1, 0, 0, 1));

    Actor *cameraActor = Actor::New(TEXT("MainCamera"));
    cameraActor->attachTo(player);
    auto camera = cameraActor->addComponent<CameraComponent>();
    camera->setMain(true);
    camera->setProjectionType(EProjectionType::Perspective);
    camera->setAspectRatio(1280.0f / 720.0f);
    camera->setHorzFov(Radian(45));
    camera->setNearClipDistance(0.1f);
    camera->setFarClipDistance(1000.0f);
    camera->getTransform()->rotate(FQuaternion(-1, 0, 0, 0));
    camera->getViewport()->setTarget(gCoreApplication().getPrimaryWindow());
    camera->getViewport()->setClearValues(Color::FromRGBA(62, 62, 84));
    camera->getViewport()->setClearFlags(EClearFlags::Color | EClearFlags::Depth | EClearFlags::Stencil);

    auto follow = cameraActor->addComponent<FollowTargetComponent>();
    follow->setTarget(player);
    follow->setOffset(Vector3(0, 0, 100));
}

int main(int argc, char **argv) {
    ApplicationStartUpDesc desc{};
    desc.renderAPI = TEXT("quark-dx11");
    desc.importers.add(TEXT("quark-assimp-importer"));
    desc.importers.add(TEXT("quark-freeimg-importer"));

    CoreApplication::StartUp(desc);

    SandboxResourceUtil::Initialize();


    setupDemoScene();


    CoreApplication::Instance().runMainLoop();

    gSceneManager().getActiveScene();

    SandboxResourceUtil::Finalization();

    CoreApplication::ShutDown();

    return EXIT_SUCCESS;
}
