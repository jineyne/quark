
#include "Resource/Resources.h"
#include "Reflection/GeneratedPrerequisites.h"

void FResources::StaticRegisterNativeFResources() {
}
IMPLEMENT_CLASS(FResources);
QClass *Generated_Initializer_Class_FResources();
static FInitClassOnStart Generated_InitClassOnStart_Class_FResources(&Generated_Initializer_Class_FResources, &FResources::StaticClass, TEXT("FResources"), TEXT("Resource/Resources.h"));

struct Generated_Class_FResources_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FResources_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FResources_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FResources_Statics::ClassDesc = {
    TEXT("FResources"),
    FResources::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FResources_Statics::ClassProperties,
    Generated_Class_FResources_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FResources() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FResources_Statics::ClassDesc);
    }
    return instance;
}
