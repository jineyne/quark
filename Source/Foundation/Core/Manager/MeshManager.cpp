#include "MeshManager.h"
#include "RenderAPI/VertexDataDesc.h"
#include "Math/Vector3.h"

void MeshManager::notifyMeshCreated(Mesh *mesh) {
    auto it = std::find(mRegisteredMeshList.begin(), mRegisteredMeshList.end(), mesh);
    if (it != mRegisteredMeshList.end()) {
        return;
    }

    mRegisteredMeshList.add(mesh);
}

void MeshManager::notifyMeshRemoved(Mesh *mesh) {
    auto it = std::find(mRegisteredMeshList.begin(), mRegisteredMeshList.end(), mesh);
    if (it == mRegisteredMeshList.end()) {
        return;
    }

    mRegisteredMeshList.remove(mesh);
}

Mesh *MeshManager::getDummyMesh() {
    if (mDummyMesh == nullptr) {
        MeshDesc desc{};
        desc.usage = EMeshUsage::Static;
        desc.vertexDesc = mDummyMeshData->getVertexDesc();

        mDummyMesh = Mesh::New(mDummyMeshData, desc);
    }

    return mDummyMesh;
}

void MeshManager::onStartUp() {
    VertexDataDesc *vertexDesc = VertexDataDesc::New();
    vertexDesc->addElement(EVertexElementType::Float3, EVertexElementSemantic::Position);

    mDummyMeshData = MeshData::New(1, 3, vertexDesc);

    auto vecIter = mDummyMeshData->getVec3DataIter(EVertexElementSemantic::Position);
    vecIter.setValue(Vector3(0, 0, 0));

    auto indices = mDummyMeshData->getIndex32();
    indices[0] = 0;
    indices[1] = 0;
    indices[2] = 0;
}

void MeshManager::onShutDown() {
    while (!mRegisteredMeshList.empty()) {
        auto mesh = mRegisteredMeshList.top();
        q_delete(mesh);
    }
}
