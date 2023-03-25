#include "Mesh.h"
#include "Manager/MeshManager.h"
#include "RenderAPI/VertexDataDesc.h"

DEFINE_LOG_CATEGORY(FLogMesh)

FMeshDesc FMeshDesc::Default = FMeshDesc();

FMesh::FMesh(const FMeshDesc &desc)
        : mVertexCount(desc.vertexCount), mIndexBuffer(nullptr), mIndexCount(desc.indexCount)
        , mVertexDesc(desc.vertexDesc), mUsage(desc.usage), mIndexType(desc.indexType) { }

FMesh::FMesh(FMeshData *data, const FMeshDesc &desc)
        : mVertexCount(data->getVertexCount()), mIndexBuffer(nullptr), mIndexCount(data->getIndexCount())
        , mVertexDesc(data->getVertexDesc()), mInitialData(data), mUsage(desc.usage), mIndexType(desc.indexType) { }

FMesh::~FMesh() {
    q_delete(mVertexData);

    FMeshManager::Instance().notifyMeshRemoved(this);
}

FMesh *FMesh::New(const FMeshDesc &desc) {
    auto mesh = q_new<FMesh>(desc);
    FMeshManager::Instance().notifyMeshCreated(mesh);
    mesh->initialize();

    return mesh;
}

FMesh *FMesh::New(FMeshData *data, const FMeshDesc &desc) {
    auto mesh = q_new<FMesh>(data, desc);
    FMeshManager::Instance().notifyMeshCreated(mesh);
    mesh->initialize();

    return mesh;
}

void FMesh::writeData(FMeshData *data, bool discardEntireBuffer) {
    if (discardEntireBuffer) {
        if (mUsage == EMeshUsage::Static) {
            LOG(FLogMesh, Warning, TEXT("Buffer discard is enabled but buffer was not created as dynamic. Disabling discard."));
            discardEntireBuffer = false;
        }
    } else {
        if (mUsage == EMeshUsage::Dynamic) {
            LOG(FLogMesh, Warning, TEXT("Buffer discard is not enabled but buffer was created as dynamic. Enabling discard."));
            discardEntireBuffer = true;
        }
    }

    auto indicesSize = data->getIndexBufferSize();
    mIndexBuffer->writeData(0, indicesSize, data->getIndexData(),
                            discardEntireBuffer ? EBufferWriteType::Discard : EBufferWriteType::Normal);

    auto myVertSize = mVertexDesc->getVertexStride();
    auto otherVertSize = data->getVertexDesc()->getVertexStride();
    if (myVertSize != otherVertSize) {
        LOG(FLogMesh, Error, TEXT("Provided index size doesn't match meshes index size. Needed: %ld. Got: %d"), myVertSize, otherVertSize);
        return;
    }

    auto vertexBuffer = mVertexData->getBuffer(0);

    auto bufferSize = data->getStreamSize();
    auto srcVertBufferData = data->getStreamData();

    if (bufferSize > vertexBuffer->getSize()) {
        bufferSize = vertexBuffer->getSize();
        LOG(FLogMesh, Error, TEXT("Index buffer values are being written out of valid range."));
    }

    vertexBuffer->writeData(0, bufferSize, srcVertBufferData,
                            discardEntireBuffer ? EBufferWriteType::Discard : EBufferWriteType::Normal);
}

FVertexData *FMesh::getVertexData() const {
    return mVertexData;
}

FIndexBuffer *FMesh::getIndexBuffer() const {
    return mIndexBuffer;
}

int FMesh::getIndexCount() const {
    return mIndexCount;
}

void FMesh::initialize() {
    FResource::initialize();

    bool isDynamic = (mUsage == EMeshUsage::Dynamic);
    auto usage = isDynamic ? EBufferUsage::Dynamic : EBufferUsage::Static;

    mVertexData = q_new<FVertexData>();
    mVertexData->vertexCount = mVertexCount;
    mVertexData->vertexDeclaration = FVertexDeclaration::New(mVertexDesc);

    FVertexBufferDesc vertexDesc{};
    vertexDesc.usage = usage;
    vertexDesc.vertexSize = mVertexData->vertexDeclaration->getVertexSize();
    vertexDesc.vertexCount = mVertexData->vertexCount;
    auto vertexBuffer = FVertexBuffer::New(vertexDesc);
    mVertexData->setBuffer(0, vertexBuffer);

    FIndexBufferDesc indexDesc{};
    indexDesc.usage = usage;
    indexDesc.indexType = mIndexType;
    indexDesc.indexCount = mIndexCount;
    mIndexBuffer = FIndexBuffer::New(indexDesc);

    if (mUsage == EMeshUsage::CpuCached) {
        EXCEPT(FLogMesh, NotImplementedException, TEXT("CPU buffer is not yet implementation"));
    }

    if (mInitialData != nullptr) {
        writeData(mInitialData, isDynamic);
        mInitialData = nullptr;
    }

    FResource::initialize();
}
