#include "RPGPrerequisites.h"


#include <Scene/Actor.h>
#include <Component/BoxCollider2DComponent.h>
#include <Component/SpriteRendererComponent.h>
#include <FileSystem/FileSystem.h>
#include <Application.h>
#include <Resource/Resources.h>
#include <Component/CameraComponent.h>
#include <Misc/Random.h>
#include <Component/SphereCollider2DComponent.h>
#include <Importer/Importer.h>
#include <Manager/InputManager.h>

class TestComponent : public Component, IInputEventListener {
private:
    BoxCollider2DComponent *mCollider;

public:
    ~TestComponent() {
        gInputManager().removeEventListener(this);
    }

public:
    void onStart() override {
        mCollider = getOwner()->getComponent<BoxCollider2DComponent>();
        mCollider->CollisionEnter.bindDynamic(TestComponent::onCollisionEnter);
        mCollider->CollisionExit.bindDynamic(TestComponent::onCollisionExit);

        gInputManager().addEventListener(this);
    }

    void onStop() override {
        mCollider->CollisionEnter.unbind(&TestComponent::onCollisionEnter, this);
        mCollider->CollisionExit.unbind(&TestComponent::onCollisionExit, this);
    }

private:
    bool onInputEvent(const InputEvent &event) override {
        if (event.keyCode == EKeyCode::A) {
            // getTransform()->setPosition({0, 100, 0});
            getOwner()->destroy();
        }

        return true;
    }

public:
    void onCollisionEnter(Collider2D *collider2D) {
        LOG(LogTemp, Info, TEXT("Collision Enter: %ls"), *collider2D->getOwner()->getName());
    }

    void onCollisionExit(Collider2D *collider2D) {
        LOG(LogTemp, Info, TEXT("Collision Exit: %ls"), *collider2D->getOwner()->getName());
    }
};

int main(int argc, char **argv) {
#if DEBUG_MODE
    FileSystem::SetWorkingDirectoryPath(ANSI_TO_TCHAR(RAW_PROJECT_ROOT));
#endif

    ApplicationStartUpDesc desc{};
    desc.renderAPI = TEXT("quark-dx11");
    desc.physicsAPI = TEXT("quark-box2d");
    desc.importers.add(TEXT("quark-assimp-importer"));
    desc.importers.add(TEXT("quark-freeimg-importer"));

    CoreApplication::StartUp(desc);

    auto cameraActor = Actor::New(TEXT("Camera"));
    auto camera = cameraActor->addComponent<CameraComponent>();
    camera->setMain(true);
    camera->setProjectionType(EProjectionType::Orthograhic);
    camera->setOrthoWindow(1280.0f, 720.0f);
    camera->setHorzFov(Radian(45));
    camera->setNearClipDistance(0.1f);
    camera->setFarClipDistance(1000.0f);
    camera->getViewport()->setTarget(gCoreApplication().getPrimaryWindow());
    camera->getViewport()->setClearValues(Color::FromRGBA(62, 62, 84));
    camera->getViewport()->setClearFlags(EClearFlags::Color | EClearFlags::Depth | EClearFlags::Stencil);
    cameraActor->getTransform()->setPosition(Vector3(0, 0, 5));


    // TODO: Initialze
    auto ground = Actor::New(TEXT("Ground"));
    auto groundCollider = ground->addComponent<BoxCollider2DComponent>();
    groundCollider->setSize({200, 10});
    auto groundSprite = ground->addComponent<SpriteRendererComponent>();
    ground->getTransform()->setScale(Vector3(200, 10, 1));

    Random rand;

    auto boxActor = Actor::New(TEXT("Box"));
    auto boxCollider = boxActor->addComponent<BoxCollider2DComponent>();
    boxCollider->setSize({10, 10});
    boxCollider->setBodyType(EPhysicsBodyType::Dynamic);
    boxActor->addComponent<TestComponent>();
    auto boxSprite = boxActor->addComponent<SpriteRendererComponent>();
    boxSprite->getTransform()->setPosition({0, 100, 0});
    boxSprite->getTransform()->setScale(Vector3(10, 10, 1));
    boxSprite->getTransform()->setRotation(FQuaternion(Vector3::Forward, 0));

    CoreApplication::Instance().runMainLoop();

    // TODO: Finalize

    CoreApplication::ShutDown();

    return EXIT_SUCCESS;
}
