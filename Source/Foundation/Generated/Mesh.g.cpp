
#include "Mesh/Mesh.h"
#include "Reflection/GeneratedPrerequisites.h"

void FMesh::StaticRegisterNativeFMesh() {
}
IMPLEMENT_CLASS(FMesh);
QClass *Generated_Initializer_Class_FMesh();
static FInitClassOnStart Generated_InitClassOnStart_Class_FMesh(&Generated_Initializer_Class_FMesh, &FMesh::StaticClass, TEXT("FMesh"), TEXT("Mesh/Mesh.h"));

struct Generated_Class_FMesh_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FMesh_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FMesh_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FMesh_Statics::ClassDesc = {
    TEXT("FMesh"),
    FMesh::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FMesh_Statics::ClassProperties,
    Generated_Class_FMesh_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FMesh() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FMesh_Statics::ClassDesc);
    }
    return instance;
}
