
#include "Scene/Component.h"
#include "Reflection/GeneratedPrerequisites.h"

void FComponent::StaticRegisterNativeFComponent() {
}
IMPLEMENT_CLASS(FComponent);
QClass *Generated_Initializer_Class_FComponent();
static FInitClassOnStart Generated_InitClassOnStart_Class_FComponent(&Generated_Initializer_Class_FComponent, &FComponent::StaticClass, TEXT("FComponent"), TEXT("Scene/Component.h"));

struct Generated_Class_FComponent_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FComponent_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FComponent_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FComponent_Statics::ClassDesc = {
    TEXT("FComponent"),
    FComponent::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FComponent_Statics::ClassProperties,
    Generated_Class_FComponent_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FComponent() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FComponent_Statics::ClassDesc);
    }
    return instance;
}
