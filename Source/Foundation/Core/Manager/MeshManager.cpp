#include "MeshManager.h"
#include "RenderAPI/VertexDataDesc.h"
#include "Math/Vector3.h"

void FMeshManager::notifyMeshCreated(FMesh *mesh) {
    auto it = std::find(mRegisteredMeshList.begin(), mRegisteredMeshList.end(), mesh);
    if (it != mRegisteredMeshList.end()) {
        return;
    }

    mRegisteredMeshList.add(mesh);
}

void FMeshManager::notifyMeshRemoved(FMesh *mesh) {
    auto it = std::find(mRegisteredMeshList.begin(), mRegisteredMeshList.end(), mesh);
    if (it == mRegisteredMeshList.end()) {
        return;
    }

    mRegisteredMeshList.remove(mesh);
}

FMesh *FMeshManager::getDummyMesh() {
    if (mDummyMesh == nullptr) {
        FMeshDesc desc{};
        desc.usage = EMeshUsage::Static;
        desc.vertexDesc = mDummyMeshData->getVertexDesc();

        mDummyMesh = FMesh::New(mDummyMeshData, desc);
    }

    return mDummyMesh;
}

void FMeshManager::onStartUp() {
    FVertexDataDesc *vertexDesc = FVertexDataDesc::New();
    vertexDesc->addElement(EVertexElementType::Float3, EVertexElementSemantic::Position);

    mDummyMeshData = FMeshData::New(1, 3, vertexDesc);

    auto vecIter = mDummyMeshData->getVec3DataIter(EVertexElementSemantic::Position);
    vecIter.setValue(FVector3(0, 0, 0));

    auto indices = mDummyMeshData->getIndex32();
    indices[0] = 0;
    indices[1] = 0;
    indices[2] = 0;
}

void FMeshManager::onShutDown() {
    for (auto &mesh : mRegisteredMeshList) {
        q_delete(mesh);
    }

    mRegisteredMeshList.clear();
}
