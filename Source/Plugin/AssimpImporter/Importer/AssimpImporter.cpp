#include "AssimpImporter.h"
#include "RenderAPI/VertexDataDesc.h"
#include "Importer/Importer.h"
#include "FileSystem/FileSystem.h"
#include "Mesh/Mesh.h"

AssimpImporter::AssimpImporter() {
    aiString _ext;
    mImporter.GetExtensionList(_ext);

    auto exts = String(_ext.data).lower();

    mExtensions = exts.split(TEXT(";"));

    // remove *.
    for (auto &ext : mExtensions) {
        ext.replace(TEXT("*."), TEXT(""));
    }

    mVertexDataDesc = VertexDataDesc::New();
    mVertexDataDesc->addElement(EVertexElementType::Float3, EVertexElementSemantic::Position);
    mVertexDataDesc->addElement(EVertexElementType::Float3, EVertexElementSemantic::Normal);
    mVertexDataDesc->addElement(EVertexElementType::Float2, EVertexElementSemantic::TexCoord);
}

Resource *AssimpImporter::import(const Path &path, const TSharedPtr<ImporterOptions> &options) {
    if (!FileSystem::Exists(path)) {
        LOG(LogImporter, Error, TEXT("Unable to find model: %ls"), *path.toString());
        return nullptr;
    }

    auto file = FileSystem::OpenFile(path);
    TArray<uint8_t> data(file->size() + 1);
    file->read(*data, file->size());
    data[file->size()] = TEXT('\0');

    auto scene = mImporter.ReadFileFromMemory(*data, data.length(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG(LogImporter, Error, *String(mImporter.GetErrorString()));
        return nullptr;
    }

    auto meshData = processNode(scene->mRootNode, scene);

    MeshDesc desc{};
    desc.usage = EMeshUsage::Static;

    auto mesh = Mesh::New(meshData, desc);
    mesh->setName(path.getFilename());

    return mesh;
}

TArray<SubResourceRaw> AssimpImporter::importAll(const Path &path, const TSharedPtr<ImporterOptions> &options) {
    auto file = FileSystem::OpenFile(path);
    TArray<uint8_t> data(file->size() + 1);
    file->read(*data, file->size());
    data[file->size()] = TEXT('\0');

    auto scene = mImporter.ReadFileFromMemory(*data, data.length(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG(LogImporter, Error, *String(mImporter.GetErrorString()));
        return {};
    }

    auto meshData = processNode(scene->mRootNode, scene);

    MeshDesc desc{};
    desc.usage = EMeshUsage::Static;

    auto mesh = Mesh::New(meshData, desc);
    mesh->setName(path.getFilename());

    TArray<SubResourceRaw> output;
    if (mesh != nullptr) {
        output.add({ TEXT("primary"), mesh });
    }

    auto shader = processMaterial(scene);
    if (shader != nullptr) {
        // output.add({ TEXT("material"), material });
    }

    return output;
}

bool AssimpImporter::isExtensionSupported(const String &ext) const {
    return mExtensions.contains(ext.lower());
}

bool AssimpImporter::isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t numBytes) const {
    return false;
}

MeshData *AssimpImporter::processNode(aiNode *node, const aiScene *scene) {
    TArray<MeshData*> meshdatas;

    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshdatas.add(processMesh(mesh, scene));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        meshdatas.add(processNode(node->mChildren[i], scene));
    }

    return MeshData::Combine(meshdatas);
}

MeshData *AssimpImporter::processMesh(aiMesh *mesh, const aiScene *scene) {
    TArray<Vector3> positions;
    TArray<Vector3> normals;
    TArray<Vector2> texCoords;

    TArray<uint32_t> indices;
    TArray<Texture *> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

        // process vertex positions, normals and texture coordinates
        Vector3 position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;
        positions.add(position);

        Vector3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;
        normals.add(normal);

        Vector2 texCoord;

        // does the mesh contain texture coordinates?
        if (mesh->mTextureCoords[0])  {
            texCoord.x = mesh->mTextureCoords[0][i].x;
            texCoord.y = mesh->mTextureCoords[0][i].y;
        }

        texCoords.add(texCoord);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.add(face.mIndices[j]);
        }
    }

    auto data = MeshData::New(positions.length(), indices.length(), mVertexDataDesc);
    data->setVertexData(EVertexElementSemantic::Position, *positions, positions.length() * sizeof(Vector3));
    data->setVertexData(EVertexElementSemantic::Normal, *normals, normals.length() * sizeof(Vector3));
    data->setVertexData(EVertexElementSemantic::TexCoord, *texCoords, texCoords.length() * sizeof(Vector2));

    memcpy(data->getIndex32(), *indices, indices.length() * sizeof(uint32_t));
    return data;
}

void *AssimpImporter::processMaterial(const aiScene *scene) {
    // auto shader = gImporter().import<Shader>(Path(_T("Standard.shader")));

    for (auto i = 0; i < scene->mNumMaterials; i++) {
        auto mat = scene->mMaterials[i];

        auto diffuses = processTexture(mat, aiTextureType_DIFFUSE);
        if (diffuses.length() > 0) {
        }

        auto specular = processTexture(mat, aiTextureType_SPECULAR);
        if (specular.length() > 0) {
        }
    }

    return nullptr;
}

TArray<FResourceHandle<Texture>> AssimpImporter::processTexture(aiMaterial *mat, aiTextureType type) {
    TArray<FResourceHandle<Texture>> textures;

    for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        auto texture = gImporter().import(Path(String(str.data)));
        texture->initialize();

        textures.add(StaticResourceCast<Texture>(texture));
    }

    return textures;
}
