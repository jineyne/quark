
#include "Scene/Actor.h"
#include "Reflection/GeneratedPrerequisites.h"

void FActor::StaticRegisterNativeFActor() {
}
IMPLEMENT_CLASS(FActor);
QClass *Generated_Initializer_Class_FActor();
static FInitClassOnStart Generated_InitClassOnStart_Class_FActor(&Generated_Initializer_Class_FActor, &FActor::StaticClass, TEXT("FActor"), TEXT("Scene/Actor.h"));

struct Generated_Class_FActor_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FActor_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FActor_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FActor_Statics::ClassDesc = {
    TEXT("FActor"),
    FActor::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FActor_Statics::ClassProperties,
    Generated_Class_FActor_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FActor() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FActor_Statics::ClassDesc);
    }
    return instance;
}
