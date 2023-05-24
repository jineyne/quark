
#include "Scene/Scene.h"
#include "Reflection/GeneratedPrerequisites.h"

void FScene::StaticRegisterNativeFScene() {
}
IMPLEMENT_CLASS(FScene);
QClass *Generated_Initializer_Class_FScene();
static FInitClassOnStart Generated_InitClassOnStart_Class_FScene(&Generated_Initializer_Class_FScene, &FScene::StaticClass, TEXT("FScene"), TEXT("Scene/Scene.h"));

struct Generated_Class_FScene_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FScene_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FScene_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FScene_Statics::ClassDesc = {
    TEXT("FScene"),
    FScene::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FScene_Statics::ClassProperties,
    Generated_Class_FScene_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FScene() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FScene_Statics::ClassDesc);
    }
    return instance;
}
