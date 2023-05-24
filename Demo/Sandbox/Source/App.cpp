#include "Application.h"

#include "RenderAPI/VertexBuffer.h"
#include "RenderAPI/GraphicsPipelineState.h"
#include "RenderAPI/GpuParams.h"
#include "RenderAPI/GpuParamBlockBuffer.h"
#include "RenderAPI/GpuProgram.h"
#include "RenderAPI/RenderAPI.h"
#include "RenderAPI/IndexBuffer.h"
#include "RenderAPI/VertexDataDesc.h"
#include "Resource/Resources.h"
#include "FileSystem/FileSystem.h"
#include "Math/Radian.h"
#include "Math/Vector3.h"
#include "Misc/Time.h"
#include "RenderAPI/SamplerState.h"
#include "Mesh/Mesh.h"
#include "Importer/Importer.h"
#include "Material/Material.h"
#include "Renderer/Renderer.h"
#include "Input/InputType.h"
#include "Manager/InputManager.h"
#include "Component/CameraComponent.h"
#include "Component/LightComponent.h"
#include "Component/MeshRendererComponent.h"
#include "Scene/Actor.h"
#include "Manager/SceneManager.h"

struct FMaterialParam {
    FColor globalAmbient = FColor::Black;
    FColor ambientColor = FColor::Black;
    FColor emissiveColor = FColor::Black;
    FColor diffuseColor = FColor::Black;
    FColor specularColor = FColor::Black;

    FColor reflectance = FColor::Black;

    float opacity = 1.f;
    float specularPower = 0.5f;
    float indexOfRefraction = 2;
    BOOL hasAmbientTexture = FALSE;

    BOOL hasEmissiveTexture = FALSE;
    BOOL hasDiffuseTexture = FALSE;
    BOOL hasSpecularTexture = FALSE;
    BOOL hasSpecularPowerTexture = FALSE;

    BOOL hasNormalTexture = FALSE;
    BOOL hasBumpTexture = FALSE;
    BOOL hasOpacityTexture = FALSE;
    float bumpIntensity = 1;

    float specularScale = 2;
    float alphaThreshold = 3;
    float padding[2];
} gMaterialParam;

FMaterial *gRedMaterial;
FMaterial *gBlueMaterial;

FResourceHandle<FTexture> gRedTexture = nullptr;
FResourceHandle<FTexture> gBlueTexture = nullptr;
FResourceHandle<FMesh> gSparrowMesh = nullptr;

FResourceHandle<FMesh> gBulletMesh = nullptr;
FResourceHandle<FTexture> gBulletTexture = nullptr;
FMaterial *gBulletMaterial;

FSamplerState *gSamplerState;

FString read(FPath path) {
    auto file = FFileSystem::OpenFile(path);
    auto size = file->size();
    char *buf = (char *) malloc(size + 1);
    file->read(buf, size);
    buf[size] = '\0';

    FString result = buf;
    delete buf;

    return result;
}

void loadShader(FPath path) {
    FPassDesc passDesc{};
    passDesc.vertexProgramDesc.type = EGpuProgramType::Vertex;
    passDesc.vertexProgramDesc.source = read(FPath::Combine(path, TEXT("/ForwardRendering.hlsl")));
    passDesc.vertexProgramDesc.entryPoint = TEXT("VSMain");

    passDesc.fragmentProgramDesc.type = EGpuProgramType::Fragment;
    passDesc.fragmentProgramDesc.source = read(FPath::Combine(path, TEXT("/ForwardRendering.hlsl")));
    passDesc.fragmentProgramDesc.entryPoint = TEXT("PSMain");

    passDesc.depthStencilStateDesc.stencilEnable = true;
    passDesc.depthStencilStateDesc.depthComparisonFunc = ECompareFunction::Less;

    passDesc.depthStencilStateDesc.depthReadEnable = true;
    passDesc.depthStencilStateDesc.depthWriteEnable = true;
    passDesc.depthStencilStateDesc.frontStencilFailOp = EStencilOperation::Keep;
    passDesc.depthStencilStateDesc.frontStencilZFailOp = EStencilOperation::Increment;
    passDesc.depthStencilStateDesc.backStencilFailOp = EStencilOperation::Keep;
    passDesc.depthStencilStateDesc.backStencilZFailOp = EStencilOperation::Decrement;

    auto pass = FPass::New(passDesc);
    pass->compile();

    auto technique = FTechnique::New("HLSL", {}, FShaderVariation(), { pass });
    FShaderDesc shaderDesc{};
    shaderDesc.techniques = { technique };
    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("Light"), TEXT("Light"), EGpuParamObjectType::StructuredBuffer));

    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("PerObject"), TEXT("PerObject"), EGpuParamObjectType::StructuredBuffer));
    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("PerCall"), TEXT("PerCall"), EGpuParamObjectType::StructuredBuffer));
    shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("DiffuseTexture"), TEXT("DiffuseTexture"), EGpuParamObjectType::Texture2D));
    if (gRenderAPI().getName() == TEXT("quark-gl")) {
        shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("LinearRepeatSampler"), TEXT("DiffuseTexture"), EGpuParamObjectType::Sampler2D));
    } else {
        shaderDesc.addParameter(FShaderObjectParamDesc(TEXT("LinearRepeatSampler"), TEXT("LinearRepeatSampler"), EGpuParamObjectType::Sampler2D));
    }

    shaderDesc.addParameter(FShaderDataParamDesc(TEXT("Mat"), TEXT("Mat"), EGpuParamDataType::Struct, 1, sizeof(gMaterialParam)));

    auto shader = FShader::New(TEXT("Default"), shaderDesc);
    gRedMaterial = FMaterial::New(shader);
    gBlueMaterial = FMaterial::New(shader);
    gBulletMaterial = FMaterial::New(shader);

    gMaterialParam.hasDiffuseTexture = true;
    gRedMaterial->setStructData(TEXT("Mat"), &gMaterialParam, sizeof(gMaterialParam));
    gBlueMaterial->setStructData(TEXT("Mat"), &gMaterialParam, sizeof(gMaterialParam));
    gBulletMaterial->setStructData(TEXT("Mat"), &gMaterialParam, sizeof(gMaterialParam));


    gRedTexture = gImporter().import<FTexture>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Texture\\StarSparrow_Red.png"));
    gRedMaterial->setTexture(TEXT("DiffuseTexture"), gRedTexture);
    gBlueTexture = gImporter().import<FTexture>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Texture\\StarSparrow_Blue.png"));
    gBlueMaterial->setTexture(TEXT("DiffuseTexture"), gBlueTexture);
    gBulletTexture = gImporter().import<FTexture>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Texture\\PolygonPrototype_Texture_01.png"));
    gBulletMaterial->setTexture(TEXT("DiffuseTexture"), gBulletTexture);

    FSamplerStateDesc samplerStateDesc{};
    samplerStateDesc.minFilter = EFilterOptions::Linear;
    samplerStateDesc.magFilter = EFilterOptions::Linear;
    samplerStateDesc.mipFilter = EFilterOptions::Linear;
    gSamplerState = FSamplerState::New(samplerStateDesc);
    gRedMaterial->setSamplerState(TEXT("LinearRepeatSampler"), gSamplerState);
    gBlueMaterial->setSamplerState(TEXT("LinearRepeatSampler"), gSamplerState);
    gBulletMaterial->setSamplerState(TEXT("LinearRepeatSampler"), gSamplerState);
}

class FShipComponent : public FComponent {
private:
    float mHealth = 10;

    float mSpeed = 0;
    float mTargetSpeed = 0;
    float mMaxSpeed = 10;
    float mAcceleration = 0.1;

    float mRotation = 0;
    float mTargetRotation = 0;

protected:
    bool bEnableRotation = true;

public:
    void onUpdate() override {
        if (mSpeed != mTargetSpeed) {
            mSpeed += std::min(FMath::Lerp<float>(gTime().getDeltaTime(), 0, mTargetSpeed), mTargetSpeed);
            mSpeed = FMath::Clamp<float>(mSpeed, 0, mTargetSpeed);
        }

        if (mSpeed != 0) {
            getTransform()->move(getTransform()->getForward() * mSpeed * gTime().getDeltaTime());
        }

        if (mRotation != mTargetRotation && bEnableRotation) {
            if (std::abs(mRotation - mTargetRotation) <= gTime().getDeltaTime()) {
                mRotation = mTargetRotation;
            } else {
                if (mRotation < mTargetRotation) {
                    mRotation += FMath::Lerp<float>(gTime().getDeltaTime(), 0, mTargetRotation);
                } else {
                    mRotation -= FMath::Lerp<float>(gTime().getDeltaTime(), 0, mTargetRotation);
                }
            }

            getTransform()->rotate(FQuaternion(mRotation, 0, 0));
        }
    }

    void setTargetSpeed(float speed) { mTargetSpeed = std::max<float>(speed, 1); }
    void addRotation(float amount) { mTargetRotation += amount; }
    void setTargetRotation(float target) { mTargetRotation = target; }
    float getRotation() const { return mRotation; }
};

FActor *player;
class FPlayerComponent : public FShipComponent {


};

class FPlayerInputComponent : public FComponent, public IInputEventListener {
    FPlayerComponent *mPlayerComponent;

    void onActive() override {
        gInputManager().addEventListener(this);

        mPlayerComponent = getOwner()->getComponent<FPlayerComponent>();
    }

    void onDeactive() override {
        gInputManager().removeEventListener(this);
    }

    bool onInputEvent(const FInputEvent &event) override {
        if (event.state == EInputState::Pressed || event.state == EInputState::Changed) {
            switch (event.keyCode) {
                case EKeyCode::W:
                    mPlayerComponent->setTargetSpeed(10);
                    break;

                case EKeyCode::S:
                    mPlayerComponent->setTargetSpeed(0);
                    break;

                case EKeyCode::A:
                    mPlayerComponent->setTargetRotation(mPlayerComponent->getRotation() - 1);
                    break;

                case EKeyCode::D:
                    mPlayerComponent->setTargetRotation(mPlayerComponent->getRotation() + 1);
                    break;
            }
        } else if (event.state == EInputState::Released) {
            switch (event.keyCode) {
                case EKeyCode::A:
                case EKeyCode::D:
                    mPlayerComponent->setTargetRotation(mPlayerComponent->getRotation());
                    break;
            }
        }

        return false;
    }
};

std::queue<FActor *> bullets;
class FBulletComponent : public FComponent {
private:
    float mLifeTime = 10;

public:
    void onUpdate() override {
        mLifeTime -= gTime().getDeltaTime();

        if (mLifeTime <= 0) {
            getOwner()->destroy();
            return;
        }

        getTransform()->move(getTransform()->getForward() * 5 * gTime().getDeltaTime());
    }
};

// TODO:
TArray<FActor *> fighters;
class FFighterAIComponent : public FShipComponent {
private:
    int mTeam;

    float mHealth = 10;
    float mSpeed = 0;

    float mBulletInterval = 0;

public:
    void setTeam(int team) {
        mTeam = team;
    }

    void fire() {
        auto bulletActor = FActor::New(TEXT("Bullet"));
        bulletActor->getTransform()->setPosition(getTransform()->getPosition());
        bulletActor->getTransform()->setRotation(getTransform()->getRotation());
        bulletActor->getTransform()->setScale(FVector3(0.01, 0.01, 0.01));

        bulletActor->addComponent<FBulletComponent>();

        auto renderer = bulletActor->addComponent<FMeshRendererComponent>();
        renderer->setMesh(gBulletMesh);
        renderer->setMaterial(gBulletMaterial);
    }

    void onCreate() override {
        FComponent::onCreate();

        bEnableRotation = false;
    }

    void onUpdate() override {
        FShipComponent::onUpdate();

        mBulletInterval -= gTime().getDeltaTime();

        if (FVector3::Distance(player->getTransform()->getPosition(), getTransform()->getPosition()) <= 100) {
            getTransform()->lookAt(player->getTransform()->getPosition(), player->getTransform()->getUp());

            auto euler = getTransform()->getRotation().toEulerAngles();
            LOG(FLogTemp, Debug, TEXT("Rot: %lf, %lf, %lf"), euler.x, euler.y, euler.z);

            if (mBulletInterval <= 0) {
                mBulletInterval = 1;

                fire();
            }
        }
    }
};

FActor *spawnFighter(int team) {
    FActor *actor = FActor::New(TEXT("Fighter"));
    actor->getTransform()->setScale(FVector3(0.01f, 0.01f, 0.01f));

    auto renderer = actor->addComponent<FMeshRendererComponent>();
    renderer->setMesh(gSparrowMesh);
    switch (team) {
        case 0:
            // ONLY FOR PLAYER
            // renderer->setMaterial(gRedMaterial);
            break;

        case 1:
            renderer->setMaterial(gBlueMaterial);
            break;
    }

    auto ai = actor->addComponent<FFighterAIComponent>();
    ai->setTeam(team);

    fighters.add(actor);

    return actor;
}

void setupPlayer() {
    player = FActor::New(TEXT("Player"));
    player->getTransform()->setScale(FVector3(0.01f, 0.01f, 0.01f));

    player->addComponent<FPlayerComponent>();
    player->addComponent<FPlayerInputComponent>();

    auto renderer = player->addComponent<FMeshRendererComponent>();
    renderer->setMaterial(gRedMaterial);
    renderer->setMesh(gSparrowMesh);
}

void setupDemoScene() {
    gSparrowMesh = gImporter().import<FMesh>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Model\\StarSparrow01.fbx"));
    gBulletMesh = gImporter().import<FMesh>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Model\\SM_Primitive_Cube_02.fbx"));

    /*for (auto i = 0; i < 10; i++) {
        auto bulletActor = FActor::New(TEXT("Bullet"));
        auto bullet = bulletActor->addComponent<FBulletComponent>();

        bulletActor->getTransform()->setScale(FVector3(0.01, 0.01, 0.01));

        auto renderer = bulletActor->addComponent<FMeshRendererComponent>();
        renderer->setMesh(gBulletMesh);
        renderer->setMaterial(gBulletMaterial);

        bullets.push(bulletActor);
    }*/

    setupPlayer();

    FActor *planeActor = spawnFighter(1);
    planeActor->getTransform()->setPosition(FVector3(0, 0, 30));
    planeActor->getTransform()->setRotation(FQuaternion(0.841471016, 0, 0.540302277, 0));

    FActor *plane2Actor = spawnFighter(1);
    plane2Actor->getTransform()->setPosition(FVector3(0, 0, -30));

    FActor *lightActor = FActor::New(TEXT("Light"));
    auto light = lightActor->addComponent<FLightComponent>();
    light->setIntensity(1);
    light->setType(ELightType::Directional);
    light->setColor(FColor::White);
    light->getTransform()->setRotation(FQuaternion(1, 0, 0, 1));

    FActor *cameraActor = FActor::New(TEXT("MainCamera"));
    cameraActor->attachTo(player);
    auto camera = cameraActor->addComponent<FCameraComponent>();
    camera->setMain(true);
    camera->setProjectionType(EProjectionType::Perspective);
    camera->setAspectRatio(1280.0f / 720.0f);
    camera->setHorzFov(FRadian(45));
    camera->setNearClipDistance(0.1f);
    camera->setFarClipDistance(1000.0f);
    camera->getTransform()->setPosition(FVector3(0, 0, 200));
    camera->getTransform()->rotate(FQuaternion(-1, 0, 0, 0));
    camera->getViewport()->setTarget(gCoreApplication().getPrimaryWindow());
    camera->getViewport()->setClearFlags(EClearFlags::Color | EClearFlags::Depth | EClearFlags::Stencil);
}

int main() {
    FApplicationStartUpDesc desc{};
    desc.renderAPI = TEXT("quark-dx11");
    desc.importers.add(TEXT("quark-assimp-importer"));
    desc.importers.add(TEXT("quark-freeimg-importer"));

    QCoreApplication::StartUp(desc);

    loadShader(FPath::Combine(FString(RAW_APP_ROOT), TEXT("Asset\\Shader\\")));

    gSceneManager().setComponentState(EComponentState::Stopped);

    setupDemoScene();

    gSceneManager().setComponentState(EComponentState::Running);

    QCoreApplication::Instance().runMainLoop();

    gBlueTexture->destroy();
    delete gBlueMaterial;
    delete gSamplerState;

    QCoreApplication::ShutDown();
}