#pragma once

#include "CorePrerequisites.h"
#include "MeshType.h"
#include "RenderAPI/IndexBuffer.h"
#include "MeshData.h"
#include "Resource/Resource.h"
#include "Mesh.g.h"

struct DLL_EXPORT FMeshDesc {
    static FMeshDesc Default;

    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;

    FVertexDataDesc *vertexDesc;

    EIndexType indexType = EIndexType::_32bit;
    EMeshUsage usage = EMeshUsage::Static;
};

QCLASS()
class DLL_EXPORT FMesh : public FResource {
    GENERATED_BODY()

private:
    uint32_t mVertexCount = 0;
    uint32_t mIndexCount = 0;

    FIndexBuffer *mIndexBuffer = nullptr;
    FVertexDataDesc *mVertexDesc = nullptr;
    FMeshData *mInitialData = nullptr;
    FVertexData *mVertexData = nullptr;

    EMeshUsage mUsage = EMeshUsage::Static;
    EIndexType mIndexType = EIndexType::_32bit;

public:
    FMesh() = default;

    FMesh(const FMeshDesc &desc);
    FMesh(FMeshData *data, const FMeshDesc &desc);

    ~FMesh();

public:
    static FMesh *New(const FMeshDesc &desc);
    static FMesh* New(FMeshData *data, const FMeshDesc &desc);

public:
    void writeData(FMeshData *data, bool discardEntireBuffer);
    FVertexData *getVertexData() const;
    FIndexBuffer *getIndexBuffer() const;

    int getIndexCount() const;
    const auto &getVertexDesc() const { return mVertexDesc; }

protected:
    void initialize() override;

private:
    friend class MeshManager;
};
