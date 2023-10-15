#include <RenderAPI/RenderAPI.h>
#include "FileSystem/FileSystem.h"
#include "SpriteRendererComponent.h"
#include "Importer/Importer.h"
#include "Manager/MeshManager.h"
#include "Scene/Actor.h"


#define AssetPath(STR) Path::Combine(Path::Combine(FileSystem::GetWorkingDirectoryPath(), TEXT("Asset/")), STR)

String Read(Path path) {
    auto file = FileSystem::OpenFile(path);
    auto size = file->size();
    char *buf = (char *) malloc(size + 1);
    file->read(buf, size);
    buf[size] = '\0';

    String result = buf;
    delete buf;

    return result;
}


Material *SpriteRendererComponent::SpriteMaterial = nullptr;

void SpriteRendererComponent::onCreate() {
    Component::onCreate();

    mInternal = q_new<Renderable>();
    mInternal->setTransform(getOwner()->getTransform());

    mInternal->initialize();

    setMesh(MeshManager::Instance().getSpriteMesh());
    if (SpriteMaterial == nullptr) {
        FPassDesc passDesc{};
        passDesc.vertexProgramDesc.type = EGpuProgramType::Vertex;
        passDesc.vertexProgramDesc.source = Read(AssetPath(TEXT("Shader/Sprite.hlsl")));
        passDesc.vertexProgramDesc.entryPoint = TEXT("VSMain");

        passDesc.fragmentProgramDesc.type = EGpuProgramType::Fragment;
        passDesc.fragmentProgramDesc.source = Read(AssetPath(TEXT("Shader/Sprite.hlsl")));
        passDesc.fragmentProgramDesc.entryPoint = TEXT("PSMain");

        passDesc.depthStencilStateDesc.stencilEnable = true;
        passDesc.depthStencilStateDesc.depthComparisonFunc = ECompareFunction::Less;

        passDesc.depthStencilStateDesc.depthReadEnable = true;
        passDesc.depthStencilStateDesc.depthWriteEnable = true;
        passDesc.depthStencilStateDesc.frontStencilFailOp = EStencilOperation::Keep;
        passDesc.depthStencilStateDesc.frontStencilZFailOp = EStencilOperation::Increment;
        passDesc.depthStencilStateDesc.backStencilFailOp = EStencilOperation::Keep;
        passDesc.depthStencilStateDesc.backStencilZFailOp = EStencilOperation::Decrement;

        auto pass = Pass::New(passDesc);
        pass->compile();

        auto technique = Technique::New("HLSL", {}, ShaderVariation(), {pass });
        ShaderDesc shaderDesc{};
        shaderDesc.techniques = { technique };

        shaderDesc.addParameter(ShaderObjectParamDesc(TEXT("PerObject"), TEXT("PerObject"), EGpuParamObjectType::StructuredBuffer));
        shaderDesc.addParameter(ShaderObjectParamDesc(TEXT("PerCall"), TEXT("PerCall"), EGpuParamObjectType::StructuredBuffer));
        shaderDesc.addParameter(ShaderObjectParamDesc(TEXT("DiffuseTexture"), TEXT("DiffuseTexture"), EGpuParamObjectType::Texture2D));
        if (gRenderAPI().getName() == TEXT("quark-gl")) {
            shaderDesc.addParameter(ShaderObjectParamDesc(TEXT("LinearRepeatSampler"), TEXT("DiffuseTexture"), EGpuParamObjectType::Sampler2D));
        } else {
            shaderDesc.addParameter(ShaderObjectParamDesc(TEXT("LinearRepeatSampler"), TEXT("LinearRepeatSampler"), EGpuParamObjectType::Sampler2D));
        }

        auto shader = Shader::New(TEXT("SpriteShader"), shaderDesc);
        SpriteMaterial = Material::New(shader);

        SpriteMaterial->setTexture(TEXT("DiffuseTexture"), Texture::White);
        SpriteMaterial->setSamplerState(TEXT("LinearRepeatSampler"), SamplerState::GetDefault());
    }
    setMaterial(SpriteMaterial);
}

void SpriteRendererComponent::onDestroy() {
    Component::onDestroy();

    q_delete(mInternal);
}

void SpriteRendererComponent::onStart() {
    Component::onStart();
}

void SpriteRendererComponent::onUpdate() {
    if (mInternal->isDirty()) {
        mInternal->updateData(EActorDirtyFlags::Everything);
    }
}

void SpriteRendererComponent::onActive() {
    mInternal->setMaterial(mMaterial);
    mInternal->setMesh(mMesh);

    mInternal->setActive(true);
    mInternal->updateData(EActorDirtyFlags::Active);
}

void SpriteRendererComponent::onDeactive() {
    if (!isDestroyed()) {
        mInternal->setActive(false);
        mInternal->updateData(EActorDirtyFlags::Active);
    }
}

void SpriteRendererComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    if ((flags & ETransformChangedFlags::Transform) == ETransformChangedFlags::Transform) {
        mInternal->updateData(EActorDirtyFlags::Transform);
    }
}

void SpriteRendererComponent::setMesh(const FResourceHandle<Mesh> &mesh) {
    mMesh = mesh;

    if (isActive()) {
        mInternal->setMesh(mesh);
    }
}

const FResourceHandle<Mesh> &SpriteRendererComponent::getMesh() const {
    return mMesh;
}

void SpriteRendererComponent::setMaterial(Material *material) {
    mMaterial = material;

    if (isActive()) {
        mInternal->setMaterial(material);
    }
}

Material *SpriteRendererComponent::getMaterial() const {
    return mMaterial;
}

void SpriteRendererComponent::setTexture(const FResourceHandle<Texture> &texture) {
    getMaterial()->setTexture(TEXT("DiffuseTexture"), texture);
}
