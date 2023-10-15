#include <Resource/Resources.h>
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

FResourceHandle<Mesh> MeshManager::getSpriteMesh() {
    if (mSpriteMesh == nullptr) {
        MeshDesc desc{};
        desc.usage = EMeshUsage::Static;
        desc.vertexCount = 4;
        desc.indexCount = 6;
        desc.vertexDesc = mSpriteMeshData->getVertexDesc();

        mSpriteMesh = StaticResourceCast<Mesh>(gResources().createResourceHandle(Mesh::New(mSpriteMeshData, desc)));
    }

    return mSpriteMesh;
}

void MeshManager::onStartUp() {
    mVertexDataDesc = VertexDataDesc::New();
    mVertexDataDesc->addElement(EVertexElementType::Float3, EVertexElementSemantic::Position);
    mVertexDataDesc->addElement(EVertexElementType::Float2, EVertexElementSemantic::TexCoord);

    mSpriteMeshData = MeshData::New(4, 6, mVertexDataDesc);

    auto vecIter = mSpriteMeshData->getVec3DataIter(EVertexElementSemantic::Position);
    vecIter.addValue(Vector3(0.5, 0.5, 0));
    vecIter.addValue(Vector3(0.5, -0.5, 0));
    vecIter.addValue(Vector3(-0.5, -0.5, 0));
    vecIter.addValue(Vector3(-0.5, 0.5, 0));

    auto texIter = mSpriteMeshData->getVec2DataIter(EVertexElementSemantic::TexCoord);
    texIter.addValue(Vector2(1, 1));
    texIter.addValue(Vector2(1, 0));
    texIter.addValue(Vector2(0, 0));
    texIter.addValue(Vector2(0, 1));

    auto indices = mSpriteMeshData->getIndex32();
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 3;
    indices[3] = 1;
    indices[4] = 2;
    indices[5] = 3;
}

void MeshManager::onShutDown() {
    mSpriteMeshData = nullptr;
    q_delete(mSpriteMeshData);

    while (!mRegisteredMeshList.empty()) {
        auto mesh = mRegisteredMeshList.top();
        q_delete(mesh);
    }
}
