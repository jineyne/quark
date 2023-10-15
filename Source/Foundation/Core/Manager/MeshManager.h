#pragma once

#include "CorePrerequisites.h"
#include "Misc/Module.h"
#include "Mesh/Mesh.h"
#include "MeshManager.g.h"

QCLASS(abstract)
class DLL_EXPORT MeshManager : public TModule<MeshManager> {
    GENERATED_BODY()

private:
    TArray<Mesh *> mRegisteredMeshList;

    VertexDataDesc *mVertexDataDesc;

    MeshData *mSpriteMeshData;
    FResourceHandle<Mesh> mSpriteMesh;

public:
    void notifyMeshCreated(Mesh *mesh);
    void notifyMeshRemoved(Mesh *mesh);

    FResourceHandle<Mesh> getSpriteMesh();

protected:
    void onStartUp() override;
    void onShutDown() override;
};
