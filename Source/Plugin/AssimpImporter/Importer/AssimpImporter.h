#pragma once

#include "AIPrerequisites.h"
#include "Importer/SpecificImporter.h"
#include "Mesh/MeshData.h"
#include "Image/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class PLUGIN_EXPORT AssimpImporter : public SpecificImporter {
private:
    TArray<String> mExtensions;
    VertexDataDesc *mVertexDataDesc;

    Assimp::Importer mImporter;

public:
    AssimpImporter();
    ~AssimpImporter() = default;

public:
    Resource *import(const Path &path, const TSharedPtr<ImporterOptions> &options) override;
    TArray<SubResourceRaw> importAll(const Path &path, const TSharedPtr<ImporterOptions> &options) override;

    bool isExtensionSupported(const String &ext) const override;
    bool isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t numBytes) const override;

protected:
    MeshData *processNode(aiNode *node, const aiScene *scene);
    MeshData *processMesh(aiMesh *mesh, const aiScene *scene);

    void *processMaterial(const aiScene *scene);
    TArray<FResourceHandle<Texture>> processTexture(aiMaterial *mat, aiTextureType type);
};