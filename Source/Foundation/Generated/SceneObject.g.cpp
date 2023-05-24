
#include "Scene/SceneObject.h"
#include "Reflection/GeneratedPrerequisites.h"

void FSceneObject::StaticRegisterNativeFSceneObject() {
}
IMPLEMENT_CLASS(FSceneObject);
QClass *Generated_Initializer_Class_FSceneObject();
static FInitClassOnStart Generated_InitClassOnStart_Class_FSceneObject(&Generated_Initializer_Class_FSceneObject, &FSceneObject::StaticClass, TEXT("FSceneObject"), TEXT("Scene/SceneObject.h"));

struct Generated_Class_FSceneObject_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FSceneObject_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FSceneObject_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FSceneObject_Statics::ClassDesc = {
    TEXT("FSceneObject"),
    FSceneObject::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FSceneObject_Statics::ClassProperties,
    Generated_Class_FSceneObject_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FSceneObject() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FSceneObject_Statics::ClassDesc);
    }
    return instance;
}
