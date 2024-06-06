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
#include <Plugin/PluginManager.h>
#include <Manager/SceneManager.h>

#include "Gui/Gui.h"

#define AssetPath(STR) Path::Combine(Path::Combine(FileSystem::GetWorkingDirectoryPath(), TEXT("Asset/")), STR)

class TestComponent : public Component, IInputEventListener {
private:
    BoxCollider2DComponent* mCollider;

    Actor *mGround = nullptr;

public:
    void onStart() override {
        mCollider = getOwner()->getComponent<BoxCollider2DComponent>();
        mCollider->CollisionEnter.bindDynamic(TestComponent::onCollisionEnter);
        mCollider->CollisionExit.bindDynamic(TestComponent::onCollisionExit);

        gInputManager().addEventListener(this);

        mGround = Actor::Find(TEXT("Ground"));
    }

    void onStop() override {
        mCollider->CollisionEnter.unbind(&TestComponent::onCollisionEnter, this);
        mCollider->CollisionExit.unbind(&TestComponent::onCollisionExit, this);
    }

    bool onInputEvent(const InputEvent &event) override {
        switch (event.keyCode) {
        case EKeyCode::A:
            getTransform()->move(Vector3::Right * -1);
            break;

        case EKeyCode::D:
            getTransform()->move(Vector3::Right);
            break;

        case EKeyCode::Escape:
            getOwner()->destroy();
            break;
        }


        return true;
    }

    void onCollisionEnter(Collider2D *collider2D) {
        LOG(LogTemp, Info, TEXT("Collision Enter: %ls"), *collider2D->getOwner()->getName());
    }

    void onCollisionExit(Collider2D *collider2D) {
        LOG(LogTemp, Info, TEXT("Collision Exit: %ls"), *collider2D->getOwner()->getName());
    }

    void onGui() override {
        Component::onGui();

        gGui().beginWindow(TEXT("Player"));
        {

            Vector3 position = getTransform()->getPosition();
            if (gGui().slider(TEXT("Position"), position, -200, 200)) {
                getTransform()->setPosition(position);
            }

            FQuaternion rotation = getTransform()->getRotation();
            if (gGui().slider(TEXT("Rotation"), rotation, -1, 1)) {
                getTransform()->setRotation(rotation);
            }

            Vector3 scale = getTransform()->getScale();
            if (gGui().slider(TEXT("Scale"), scale, -200, 200)) {
                getTransform()->setScale(scale);
            }

        }
        gGui().endWindow();

        gGui().beginWindow(TEXT("Ground"));

        {
            Vector3 position = mGround->getTransform()->getPosition();
            if (gGui().slider(TEXT("Position"), position, -200, 200)) {
                mGround->getTransform()->setPosition(position);
            }

            FQuaternion rotation = mGround->getTransform()->getRotation();
            if (gGui().slider(TEXT("Rotation"), rotation, -1, 1)) {
                mGround->getTransform()->setRotation(rotation);
            }

            Vector3 scale = mGround->getTransform()->getScale();
            if (gGui().slider(TEXT("Scale"), scale, -200, 200)) {
                mGround->getTransform()->setScale(scale);
            }
        }

        gGui().endWindow();
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

    _ASSERTE( _CrtCheckMemory( ) );

    auto quarkImguiPluginId = gPluginManager().loadPlugin(TEXT("quark-imgui"));

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


    Sprite* whiteSprite = Sprite::New();
    whiteSprite->setTexture(Texture::White);
    whiteSprite->setBound(Rect(1, 1));

    // TODO: Initialze
    auto ground = Actor::New(TEXT("Ground"));
    auto groundCollider = ground->addComponent<BoxCollider2DComponent>();
    groundCollider->setSize({200, 10});
    auto groundSprite = ground->addComponent<SpriteRendererComponent>();
    ground->getTransform()->setScale(Vector3(200, 10, 1));
    groundSprite->setSprite(whiteSprite);

    Random rand;

    auto boxActor = Actor::New(TEXT("Box"));
    auto boxCollider = boxActor->addComponent<BoxCollider2DComponent>();
    boxCollider->setSize({80, 78});
    boxCollider->setBodyType(EPhysicsBodyType::Dynamic);
    boxActor->addComponent<TestComponent>();
    auto boxSprite = boxActor->addComponent<SpriteRendererComponent>();
    boxSprite->getTransform()->setPosition({0, 100, 0});
    boxSprite->getTransform()->setScale(Vector3(80, 78, 1));
    boxSprite->getTransform()->setRotation(FQuaternion(Vector3::Forward, 0));

    auto sprite = Sprite::New();
    sprite->setTexture(gResources().load<Texture>(TEXT("Texture/idle.png")));
    sprite->setBound(Rect(80, 78));
    boxSprite->setSprite(sprite);

    CoreApplication::Instance().runMainLoop();

    // TODO: Finalize

    gPluginManager().unloadPlugin(quarkImguiPluginId);

    CoreApplication::ShutDown();

    return EXIT_SUCCESS;
}
