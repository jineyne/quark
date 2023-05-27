#include "Mesh.h"
#include "Manager/MeshManager.h"
#include "RenderAPI/VertexDataDesc.h"

DEFINE_LOG_CATEGORY(LogMesh)

MeshDesc MeshDesc::Default = MeshDesc();

Mesh::Mesh(const MeshDesc &desc)
        : mVertexCount(desc.vertexCount), mIndexBuffer(nullptr), mIndexCount(desc.indexCount)
        , mVertexDesc(desc.vertexDesc), mUsage(desc.usage), mIndexType(desc.indexType) { }

Mesh::Mesh(MeshData *data, const MeshDesc &desc)
        : mVertexCount(data->getVertexCount()), mIndexBuffer(nullptr), mIndexCount(data->getIndexCount())
        , mVertexDesc(data->getVertexDesc()), mInitialData(data), mUsage(desc.usage), mIndexType(desc.indexType) { }

Mesh::~Mesh() {
    q_delete(mVertexData);

    MeshManager::Instance().notifyMeshRemoved(this);
}

Mesh *Mesh::New(const MeshDesc &desc) {
    auto mesh = q_new<Mesh>(desc);
    MeshManager::Instance().notifyMeshCreated(mesh);
    mesh->initialize();

    return mesh;
}

Mesh *Mesh::New(MeshData *data, const MeshDesc &desc) {
    auto mesh = q_new<Mesh>(data, desc);
    MeshManager::Instance().notifyMeshCreated(mesh);
    mesh->initialize();

    return mesh;
}

void Mesh::writeData(MeshData *data, bool discardEntireBuffer) {
    if (discardEntireBuffer) {
        if (mUsage == EMeshUsage::Static) {
            LOG(LogMesh, Warning, TEXT("Buffer discard is enabled but buffer was not created as dynamic. Disabling discard."));
            discardEntireBuffer = false;
        }
    } else {
        if (mUsage == EMeshUsage::Dynamic) {
            LOG(LogMesh, Warning, TEXT("Buffer discard is not enabled but buffer was created as dynamic. Enabling discard."));
            discardEntireBuffer = true;
        }
    }

    auto indicesSize = data->getIndexBufferSize();
    mIndexBuffer->writeData(0, indicesSize, data->getIndexData(),
                            discardEntireBuffer ? EBufferWriteType::Discard : EBufferWriteType::Normal);

    auto myVertSize = mVertexDesc->getVertexStride();
    auto otherVertSize = data->getVertexDesc()->getVertexStride();
    if (myVertSize != otherVertSize) {
        LOG(LogMesh, Error, TEXT("Provided index size doesn't match meshes index size. Needed: %ld. Got: %d"), myVertSize, otherVertSize);
        return;
    }

    auto vertexBuffer = mVertexData->getBuffer(0);

    auto bufferSize = data->getStreamSize();
    auto srcVertBufferData = data->getStreamData();

    if (bufferSize > vertexBuffer->getSize()) {
        bufferSize = vertexBuffer->getSize();
        LOG(LogMesh, Error, TEXT("Index buffer values are being written out of valid range."));
    }

    vertexBuffer->writeData(0, bufferSize, srcVertBufferData,
                            discardEntireBuffer ? EBufferWriteType::Discard : EBufferWriteType::Normal);
}

VertexData *Mesh::getVertexData() const {
    return mVertexData;
}

IndexBuffer *Mesh::getIndexBuffer() const {
    return mIndexBuffer;
}

int Mesh::getIndexCount() const {
    return mIndexCount;
}

void Mesh::initialize() {
    Resource::initialize();

    bool isDynamic = (mUsage == EMeshUsage::Dynamic);
    auto usage = isDynamic ? EBufferUsage::Dynamic : EBufferUsage::Static;

    mVertexData = q_new<VertexData>();
    mVertexData->vertexCount = mVertexCount;
    mVertexData->vertexDeclaration = VertexDeclaration::New(mVertexDesc);

    VertexBufferDesc vertexDesc{};
    vertexDesc.usage = usage;
    vertexDesc.vertexSize = mVertexData->vertexDeclaration->getVertexSize();
    vertexDesc.vertexCount = mVertexData->vertexCount;
    auto vertexBuffer = VertexBuffer::New(vertexDesc);
    mVertexData->setBuffer(0, vertexBuffer);

    FIndexBufferDesc indexDesc{};
    indexDesc.usage = usage;
    indexDesc.indexType = mIndexType;
    indexDesc.indexCount = mIndexCount;
    mIndexBuffer = IndexBuffer::New(indexDesc);

    if (mUsage == EMeshUsage::CpuCached) {
        EXCEPT(LogMesh, NotImplementedException, TEXT("CPU buffer is not yet implementation"));
    }

    if (mInitialData != nullptr) {
        writeData(mInitialData, isDynamic);
        mInitialData = nullptr;
    }

    Resource::initialize();
}
