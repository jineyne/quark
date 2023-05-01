#include "CoreApplication.h"

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
#include "Manager/RenderWindowManager.h"
#include "RenderAPI/SamplerState.h"
#include "Mesh/Mesh.h"
#include "Importer/Importer.h"
#include "Material/Material.h"
#include "Renderer/Renderable.h"
#include "Renderer/Renderer.h"
#include "Input/InputType.h"
#include "Input/InputManager.h"

struct FLightParamDef {
    FColor ambientColor;
    FColor diffuseColor;
    FVector3 lightDirection;
    float padding;
} gLightParam;

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

FMaterial *gMaterial;
FGpuParamBlockBuffer *gMaterialBuffer;

FResourceHandle<FTexture> gTexture = nullptr;
FResourceHandle<FMesh> gMesh;

FSamplerState *gSamplerState;
FRenderable *gRenderable;

FTransform *gTransform;
FCameraBase *gMainCamera;
FLightBase *gMainLight;

FString read(FString path) {
    auto file = FFileSystem::OpenFile(path);
    auto size = file->size();
    char *buf = (char *) malloc(size + 1);
    file->read(buf, size);
    buf[size] = '\0';

    FString result = buf;
    delete buf;

    return result;
}

void loadShader(FString path) {
    FPassDesc passDesc{};
    passDesc.vertexProgramDesc.type = EGpuProgramType::Vertex;
    passDesc.vertexProgramDesc.source = read(path + "/ForwardRendering.hlsl");
    passDesc.vertexProgramDesc.entryPoint = TEXT("VSMain");

    passDesc.fragmentProgramDesc.type = EGpuProgramType::Fragment;
    passDesc.fragmentProgramDesc.source = read(path + "/ForwardRendering.hlsl");
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
    gMaterial = FMaterial::New(shader);

    gMaterialParam.hasDiffuseTexture = true;
    gMaterial->setStructData(TEXT("Mat"), &gMaterialParam, sizeof(gMaterialParam));

    FSamplerStateDesc samplerStateDesc{};
    samplerStateDesc.minFilter = EFilterOptions::Linear;
    samplerStateDesc.magFilter = EFilterOptions::Linear;
    samplerStateDesc.mipFilter = EFilterOptions::Linear;
    gSamplerState = FSamplerState::New(samplerStateDesc);
    gMaterial->setSamplerState(TEXT("LinearRepeatSampler"), gSamplerState);

    // updateTexture(gTextureWidth, gTextureHeight);
    // gMaterial->setBuffer(TEXT("Light"), );

    gRenderable->setMaterial(gMaterial);
}

void loadMesh() {
    gMesh = gImporter().import<FMesh>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Model\\SM_Primitive_Cube_02.fbx"));
    gTexture = gImporter().import<FTexture>(TEXT("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Texture\\PolygonPrototype_Texture_01.png"));

    gRenderable->setMesh(gMesh.get());
    gMaterial->setTexture(TEXT("DiffuseTexture"), gTexture);
}

double value = 0;
FTransform *gCameraTransform = nullptr;

struct FDemoInput : public IInputEventListener {
    FVector3 prevMousePosition;

    bool onInputEvent(const FInputEvent &event) override {
        if (event.deviceType == EInputDeviceType::Keyboard) {
            onKeyboardInput(event);
        } else if (event.deviceType == EInputDeviceType::Mouse) {
            onMouseInput(event);
        }

        return false;
    }

    void onKeyboardInput(const FInputEvent &event) {
        if (event.state == EInputState::Pressed || event.state == EInputState::Changed) {
            switch (event.keyCode) {
                case EKeyCode::W:
                    gCameraTransform->move(FVector3(0, 0, -1));
                    gCameraTransform->setDirty();
                    gMainCamera->setDirty();
                    gRenderer().notifyCameraUpdated(gMainCamera);
                    LOG(LogTemp, Info, TEXT("Move Forward, Camera = %lf, %lf, %lf"), gCameraTransform->getPosition().x, gCameraTransform->getPosition().y, gCameraTransform->getPosition().z);
                    break;
                case EKeyCode::A:
                    gCameraTransform->move(FVector3(1, 0, 0));
                    gCameraTransform->setDirty();
                    gMainCamera->setDirty();
                    gRenderer().notifyCameraUpdated(gMainCamera);
                    LOG(LogTemp, Info, TEXT("Move Left, Camera = %lf, %lf, %lf"), gCameraTransform->getPosition().x, gCameraTransform->getPosition().y, gCameraTransform->getPosition().z);
                    break;
                case EKeyCode::S:
                    gCameraTransform->move(FVector3(0, 0, 1));
                    gCameraTransform->setDirty();
                    gMainCamera->setDirty();
                    gRenderer().notifyCameraUpdated(gMainCamera);
                    LOG(LogTemp, Info, TEXT("Move Backword, Camera = %lf, %lf, %lf"), gCameraTransform->getPosition().x, gCameraTransform->getPosition().y, gCameraTransform->getPosition().z);
                    break;
                case EKeyCode::D:
                    gCameraTransform->move(FVector3(-1, 0, 0));
                    gCameraTransform->setDirty();
                    gMainCamera->setDirty();
                    gRenderer().notifyCameraUpdated(gMainCamera);
                    LOG(LogTemp, Info, TEXT("Move Right, Camera = %lf, %lf, %lf"), gCameraTransform->getPosition().x, gCameraTransform->getPosition().y, gCameraTransform->getPosition().z);
                    break;

                case EKeyCode::LeftArrow:
                    gCameraTransform->rotate(FQuaternion(0, -0.01, 0));
                    LOG(LogTemp, Info, TEXT("Left, Rotation = %lf, %lf, %lf"), gCameraTransform->getRotation().toEulerAngles().x, gCameraTransform->getRotation().toEulerAngles().y, gCameraTransform->getRotation().toEulerAngles().z);
                    gCameraTransform->setDirty();
                    gMainCamera->setDirty();
                    gRenderer().notifyCameraUpdated(gMainCamera);
                    break;

                case EKeyCode::RightArrow:
                    gCameraTransform->rotate(FQuaternion(0, 0.01, 0));
                    LOG(LogTemp, Info, TEXT("Right, Rotation = %lf, %lf, %lf"), gCameraTransform->getRotation().toEulerAngles().x, gCameraTransform->getRotation().toEulerAngles().y, gCameraTransform->getRotation().toEulerAngles().z);
                    gCameraTransform->setDirty();
                    gMainCamera->setDirty();
                    gRenderer().notifyCameraUpdated(gMainCamera);
                    break;

                case EKeyCode::UpArrow:
                    gCameraTransform->rotate(FQuaternion(0.01, 0, 0));
                    LOG(LogTemp, Info, TEXT("Up, Rotation = %lf, %lf, %lf"), gCameraTransform->getRotation().toEulerAngles().x, gCameraTransform->getRotation().toEulerAngles().y, gCameraTransform->getRotation().toEulerAngles().z);
                    gCameraTransform->setDirty();
                    gMainCamera->setDirty();
                    gRenderer().notifyCameraUpdated(gMainCamera);
                    break;

                case EKeyCode::DownArrow:
                    gCameraTransform->rotate(FQuaternion(-0.01, 0, 0));
                    LOG(LogTemp, Info, TEXT("Down, Rotation = %lf, %lf, %lf"), gCameraTransform->getRotation().toEulerAngles().x, gCameraTransform->getRotation().toEulerAngles().y, gCameraTransform->getRotation().toEulerAngles().z);
                    gCameraTransform->setDirty();
                    gMainCamera->setDirty();
                    gRenderer().notifyCameraUpdated(gMainCamera);
                    break;

            }
        }
    }

    FVector2 mPrevMousePosition;
    bool bMousePressed = false;
    void onMouseInput(const FInputEvent &event) {
        if (event.state == EInputState::Pressed) {
            // begin
            bMousePressed = true;
            mPrevMousePosition = {0, 0};
        } else if (event.state == EInputState::Released) {
            bMousePressed = false;
        } else if (event.state == EInputState::Changed) {
            if (bMousePressed) {
                // LOG(LogTemp, Info, TEXT("event.value: %lf"), event.value)
                if (event.keyCode == EKeyCode::MouseX) {
                    if (mPrevMousePosition.x == 0) {
                        mPrevMousePosition.x = event.value;
                    }

                    float diff = event.value - mPrevMousePosition.x;
                    mPrevMousePosition.x = event.value;

                    gCameraTransform->rotate(FQuaternion(0, diff * gTime().getDeltaTime() * 0.001, 0));
                } else if (event.keyCode == EKeyCode::MouseY) {
                    if (mPrevMousePosition.y == 0) {
                        mPrevMousePosition.y = event.value;
                    }

                    float diff = event.value - mPrevMousePosition.y;
                    mPrevMousePosition.y = event.value;

                    gCameraTransform->rotate(FQuaternion(diff * gTime().getDeltaTime() * 0.001, 0, 0));
                }

                LOG(LogTemp, Info, TEXT("Mouse Moved, Rotation = %lf, %lf, %lf"), gCameraTransform->getRotation().toEulerAngles().x, gCameraTransform->getRotation().toEulerAngles().y, gCameraTransform->getRotation().toEulerAngles().z);

                gCameraTransform->setDirty();
                gMainCamera->setDirty();
                gRenderer().notifyCameraUpdated(gMainCamera);
            }
        }
    }
};

int main() {
    FApplicationStartUpDesc desc{};
    desc.renderAPI = TEXT("quark-dx11");
    desc.importers.add(TEXT("quark-assimp-importer"));
    desc.importers.add(TEXT("quark-freeimg-importer"));

    QCoreApplication::StartUp(desc);

    FDemoInput input;
    gInputManager().addEventListener(&input);

    gTransform = q_new<FTransform>(nullptr);
    gTransform->setPosition(FVector3(0, 0, 100));
    gTransform->setScale(FVector3(0.25f, 0.25f, 0.25f));

    gRenderable = q_new<FRenderable>();
    gRenderable->setTransform(gTransform);

    gMainCamera = q_new<FCameraBase>();
    gMainCamera->setMain(true);
    gMainCamera->setProjectionType(EProjectionType::Perspective);
    gMainCamera->setAspectRatio(1280.0f / 720.0f);
    gMainCamera->setHorzFov(FRadian(45));
    gMainCamera->setNearClipDistance(0.1f);
    gMainCamera->setFarClipDistance(1000.0f);
    gCameraTransform = q_new<FTransform>(nullptr);
    gCameraTransform->setPosition(FVector3(0, 0, 0));
    gMainCamera->setTransform(gCameraTransform);
    gMainCamera->getViewport()->setTarget(gCoreApplication().getPrimaryWindow());
    gMainCamera->getViewport()->setClearFlags(EClearFlags::Color | EClearFlags::Depth | EClearFlags::Stencil);

    loadShader("D:\\Projects\\Quark\\Demo\\Sandbox\\Asset\\Shader");
    loadMesh();

    gMainLight = q_new<FLightBase>();
    gMainLight->setIntensity(1);
    gMainLight->setType(ELightType::Directional);
    gMainLight->setColor(FColor::White);
    gMainLight->setTransform(q_new<FTransform>(nullptr));
    gMainLight->getTransform()->rotate(FQuaternion(0, 0, 0));

    gRenderable->initialize();
    gMainCamera->initialize();
    gMainLight->initialize();

    gRenderable->update(EActorDirtyFlags::Transform);

    QCoreApplication::Instance().runMainLoop();

    gTexture->destroy();
    delete gMaterial;
    delete gSamplerState;

    QCoreApplication::ShutDown();
}