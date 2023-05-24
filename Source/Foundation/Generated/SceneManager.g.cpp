
#include "Manager/SceneManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FSceneManager::StaticRegisterNativeFSceneManager() {
}
IMPLEMENT_CLASS(FSceneManager);
QClass *Generated_Initializer_Class_FSceneManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FSceneManager(&Generated_Initializer_Class_FSceneManager, &FSceneManager::StaticClass, TEXT("FSceneManager"), TEXT("Manager/SceneManager.h"));

struct Generated_Class_FSceneManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FSceneManager_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FSceneManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FSceneManager_Statics::ClassDesc = {
    TEXT("FSceneManager"),
    FSceneManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FSceneManager_Statics::ClassProperties,
    Generated_Class_FSceneManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FSceneManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FSceneManager_Statics::ClassDesc);
    }
    return instance;
}
