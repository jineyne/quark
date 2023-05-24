
#include "D:/Projects/Quark/Source/Foundation/Core/Component/MeshRendererComponent.h"
#include "Reflection/GeneratedPrerequisites.h"

void FMeshRendererComponent::StaticRegisterNativeFMeshRendererComponent() {
}
IMPLEMENT_CLASS(FMeshRendererComponent);
QClass *Generated_Initializer_Class_FMeshRendererComponent();
static FInitClassOnStart Generated_InitClassOnStart_Class_FMeshRendererComponent(&Generated_Initializer_Class_FMeshRendererComponent, &FMeshRendererComponent::StaticClass, TEXT("FMeshRendererComponent"), TEXT("D:/Projects/Quark/Source/Foundation/Core/Component/MeshRendererComponent.h"));

struct Generated_Class_FMeshRendererComponent_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FMeshRendererComponent_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FMeshRendererComponent_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FMeshRendererComponent_Statics::ClassDesc = {
    TEXT("FMeshRendererComponent"),
    FMeshRendererComponent::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FMeshRendererComponent_Statics::ClassProperties,
    Generated_Class_FMeshRendererComponent_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FMeshRendererComponent() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FMeshRendererComponent_Statics::ClassDesc);
    }
    return instance;
}
