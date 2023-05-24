
#include "Manager/SceneObjectManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FSceneObjectManager::StaticRegisterNativeFSceneObjectManager() {
}
IMPLEMENT_CLASS(FSceneObjectManager);
QClass *Generated_Initializer_Class_FSceneObjectManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FSceneObjectManager(&Generated_Initializer_Class_FSceneObjectManager, &FSceneObjectManager::StaticClass, TEXT("FSceneObjectManager"), TEXT("Manager/SceneObjectManager.h"));

struct Generated_Class_FSceneObjectManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FSceneObjectManager_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FSceneObjectManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FSceneObjectManager_Statics::ClassDesc = {
    TEXT("FSceneObjectManager"),
    FSceneObjectManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FSceneObjectManager_Statics::ClassProperties,
    Generated_Class_FSceneObjectManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FSceneObjectManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FSceneObjectManager_Statics::ClassDesc);
    }
    return instance;
}
