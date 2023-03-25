#pragma once

#include "CorePrerequisites.h"
#include "Misc/Module.h"
#include "Mesh/Mesh.h"
#include "MeshManager.g.h"

QCLASS(abstract)
class DLL_EXPORT FMeshManager : public TModule<FMeshManager> {
    GENERATED_BODY()

private:
    TArray<FMesh *> mRegisteredMeshList;

    FMeshData *mDummyMeshData;
    FMesh *mDummyMesh;

public:
    void notifyMeshCreated(FMesh *mesh);
    void notifyMeshRemoved(FMesh *mesh);

    FMeshData *getDummyMeshData() const { return mDummyMeshData; }
    FMesh *getDummyMesh();

protected:
    void onStartUp() override;
    void onShutDown() override;
};
