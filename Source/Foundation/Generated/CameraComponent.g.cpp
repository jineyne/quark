
#include "Component/CameraComponent.h"
#include "Reflection/GeneratedPrerequisites.h"

void FCameraComponent::StaticRegisterNativeFCameraComponent() {
}
IMPLEMENT_CLASS(FCameraComponent);
QClass *Generated_Initializer_Class_FCameraComponent();
static FInitClassOnStart Generated_InitClassOnStart_Class_FCameraComponent(&Generated_Initializer_Class_FCameraComponent, &FCameraComponent::StaticClass, TEXT("FCameraComponent"), TEXT("Component/CameraComponent.h"));

struct Generated_Class_FCameraComponent_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FCameraComponent_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FCameraComponent_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FCameraComponent_Statics::ClassDesc = {
    TEXT("FCameraComponent"),
    FCameraComponent::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FCameraComponent_Statics::ClassProperties,
    Generated_Class_FCameraComponent_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FCameraComponent() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FCameraComponent_Statics::ClassDesc);
    }
    return instance;
}
