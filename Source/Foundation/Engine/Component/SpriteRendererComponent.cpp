#include "SpriteRendererComponent.h"

#include "RenderAPI/VertexDataDesc.h"
#include "RenderAPI/RenderAPI.h"
#include "FileSystem/FileSystem.h"
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

Shader *SpriteRendererComponent::SpriteShader = nullptr;

void SpriteRendererComponent::onCreate() {
    Component::onCreate();

    mVertexDataDesc = VertexDataDesc::New();
    mVertexDataDesc->addElement(EVertexElementType::Float3, EVertexElementSemantic::Position);
    mVertexDataDesc->addElement(EVertexElementType::Float2, EVertexElementSemantic::TexCoord);
    mMeshData = MeshData::New(4, 6, mVertexDataDesc);

    MeshDesc meshDesc{};
    meshDesc.vertexCount = mMeshData->getVertexCount();
    meshDesc.indexCount = mMeshData->getIndexCount();
    meshDesc.vertexDesc = mVertexDataDesc;
    meshDesc.usage = EMeshUsage::Dynamic;

    mMesh = Mesh::New(meshDesc);
    mMesh->writeData(mMeshData, true);

    if (SpriteShader == nullptr) {
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

        passDesc.blendStateDesc.renderTargetDesc.blendEnable = true;

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
        mMaterial = Material::New(shader);

        mMaterial->setTexture(TEXT("DiffuseTexture"), Texture::White);
        mMaterial->setSamplerState(TEXT("LinearRepeatSampler"), SamplerState::GetDefault());
    }

    if (mSprite != nullptr) {
        mInternal = q_new<Renderable>();
        mInternal->setTransform(getOwner()->getTransform());

        mInternal->setMesh(mMesh);
        setMaterial(mMaterial);

        mInternal->initialize();
    } else {
        mInternal = nullptr;
    }
}

void SpriteRendererComponent::onDestroy() {
    Component::onDestroy();

    q_delete(mInternal);
}

void SpriteRendererComponent::onStart() {
    Component::onStart();
}

void SpriteRendererComponent::onUpdate() {
    if (mInternal != nullptr && mInternal->isDirty()) {
        mInternal->updateData(EActorDirtyFlags::Everything);
    }

    if (bIsDirty) {
        float texWidth = mSprite->getTexture()->getWidth();
        float texHeight = mSprite->getTexture()->getHeight();
        auto spriteBound = mSprite->getBound();
        Rect bound = {
                spriteBound.left() / texWidth,
                spriteBound.bottom() / texHeight,
                spriteBound.right() / texWidth,
                spriteBound.top() / texHeight,
        };

        auto vecIter = mMeshData->getVec3DataIter(EVertexElementSemantic::Position);
        vecIter.addValue(Vector3( 0.5,  0.5, 0));
        vecIter.addValue(Vector3( 0.5, -0.5, 0));
        vecIter.addValue(Vector3(-0.5, -0.5, 0));
        vecIter.addValue(Vector3(-0.5,  0.5, 0));

        auto texIter = mMeshData->getVec2DataIter(EVertexElementSemantic::TexCoord);
        texIter.addValue(Vector2(bound.left(), bound.bottom()));
        texIter.addValue(Vector2(bound.left(), bound.top()));
        texIter.addValue(Vector2(bound.right(), bound.top()));
        texIter.addValue(Vector2(bound.right(), bound.bottom()));

        auto indices = mMeshData->getIndex32();
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 3;
        indices[3] = 1;
        indices[4] = 2;
        indices[5] = 3;

        mMesh->writeData(mMeshData, true);
        bIsDirty = false;

        if (mInternal == nullptr) {
            mInternal = q_new<Renderable>();
            mInternal->setTransform(getOwner()->getTransform());

            mInternal->setMesh(mMesh);
            mInternal->setMaterial(mMaterial);

            mInternal->initialize();
        }
    }
}

void SpriteRendererComponent::onActive() {
    if (mInternal != nullptr) {
        mInternal->setMaterial(mMaterial);
        mInternal->setMesh(mMesh);

        mInternal->setActive(true);
        mInternal->updateData(EActorDirtyFlags::Active);
    }
}

void SpriteRendererComponent::onDeactive() {
    if (mInternal != nullptr && !isDestroyed()) {
        mInternal->setActive(false);
        mInternal->updateData(EActorDirtyFlags::Active);
    }
}

void SpriteRendererComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    if (mInternal != nullptr && (flags & ETransformChangedFlags::Transform) == ETransformChangedFlags::Transform) {
        mInternal->updateData(EActorDirtyFlags::Transform);
    }
}

void SpriteRendererComponent::setMaterial(Material *material) {
    mMaterial = material;

    if (mInternal != nullptr && isActive()) {
        mInternal->setMaterial(material);
    }
}

Material *SpriteRendererComponent::getMaterial() const {
    return mMaterial;
}

Sprite *SpriteRendererComponent::getSprite() const {
    return mSprite;
}

void SpriteRendererComponent::setSprite(Sprite *sprite) {
    if (mSprite == sprite) {
        return;
    }

    mSprite = sprite;
    bIsDirty = true;

    mMaterial->setTexture(TEXT("DiffuseTexture"), mSprite->getTexture());
}
