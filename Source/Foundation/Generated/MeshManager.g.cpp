
#include "Manager/MeshManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FMeshManager::StaticRegisterNativeFMeshManager() {
}
IMPLEMENT_CLASS_NO_CTR(FMeshManager);
QClass *Generated_Initializer_Class_FMeshManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FMeshManager(&Generated_Initializer_Class_FMeshManager, &FMeshManager::StaticClass, TEXT("FMeshManager"), TEXT("Manager/MeshManager.h"));

struct Generated_Class_FMeshManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FMeshManager_Statics::ClassMetaData = {
    {TEXT("abstract"), TEXT("true")},
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FMeshManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FMeshManager_Statics::ClassDesc = {
    TEXT("FMeshManager"),
    FMeshManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FMeshManager_Statics::ClassProperties,
    Generated_Class_FMeshManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FMeshManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FMeshManager_Statics::ClassDesc);
    }
    return instance;
}
