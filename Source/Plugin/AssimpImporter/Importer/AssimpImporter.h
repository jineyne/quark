#pragma once

#include "AIPrerequisites.h"
#include "Importer/SpecificImporter.h"
#include "Mesh/MeshData.h"
#include "Image/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class PLUGIN_EXPORT FAssimpImporter : public FSpecificImporter {
private:
    TArray<FString> mExtensions;
    FVertexDataDesc *mVertexDataDesc;

    Assimp::Importer mImporter;

public:
    FAssimpImporter();
    ~FAssimpImporter() = default;

public:
    FResource *import(const FPath &path, const TSharedPtr<FImporterOptions> &options) override;
    TArray<SubResourceRaw> importAll(const FPath &path, const TSharedPtr<FImporterOptions> &options) override;

    bool isExtensionSupported(const FString &ext) const override;
    bool isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t numBytes) const override;

protected:
    FMeshData *processNode(aiNode *node, const aiScene *scene);
    FMeshData *processMesh(aiMesh *mesh, const aiScene *scene);

    void *processMaterial(const aiScene *scene);
    TArray<FResourceHandle<FTexture>> processTexture(aiMaterial *mat, aiTextureType type);
};