#pragma once

#include "CorePrerequisites.h"
#include "MeshType.h"
#include "RenderAPI/IndexBuffer.h"
#include "MeshData.h"
#include "Resource/Resource.h"
#include "Mesh.g.h"

struct DLL_EXPORT MeshDesc {
    static MeshDesc Default;

    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;

    VertexDataDesc *vertexDesc;

    EIndexType indexType = EIndexType::_32bit;
    EMeshUsage usage = EMeshUsage::Static;
};

QCLASS()
class DLL_EXPORT Mesh : public Resource {
    GENERATED_BODY()

private:
    uint32_t mVertexCount = 0;
    uint32_t mIndexCount = 0;

    IndexBuffer *mIndexBuffer = nullptr;
    VertexDataDesc *mVertexDesc = nullptr;
    MeshData *mInitialData = nullptr;
    VertexData *mVertexData = nullptr;

    EMeshUsage mUsage = EMeshUsage::Static;
    EIndexType mIndexType = EIndexType::_32bit;

public:
    Mesh() = default;

    Mesh(const MeshDesc &desc);
    Mesh(MeshData *data, const MeshDesc &desc);

    ~Mesh();

public:
    static Mesh *New(const MeshDesc &desc);
    static Mesh* New(MeshData *data, const MeshDesc &desc);

public:
    void writeData(MeshData *data, bool discardEntireBuffer);
    VertexData *getVertexData() const;
    IndexBuffer *getIndexBuffer() const;

    int getIndexCount() const;
    const auto &getVertexDesc() const { return mVertexDesc; }

protected:
    void initialize() override;

private:
    friend class MeshManager;
};
