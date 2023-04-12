
#include "Renderer/Renderer.h"
#include "Reflection/GeneratedPrerequisites.h"

void FRenderer::StaticRegisterNativeFRenderer() {
}
IMPLEMENT_CLASS(FRenderer);
QClass *Generated_Initializer_Class_FRenderer();
static FInitClassOnStart Generated_InitClassOnStart_Class_FRenderer(&Generated_Initializer_Class_FRenderer, &FRenderer::StaticClass, TEXT("FRenderer"), TEXT("Renderer/Renderer.h"));

struct Generated_Class_FRenderer_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FRenderer_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FRenderer_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FRenderer_Statics::ClassDesc = {
    TEXT("FRenderer"),
    FRenderer::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FRenderer_Statics::ClassProperties,
    Generated_Class_FRenderer_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FRenderer() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FRenderer_Statics::ClassDesc);
    }
    return instance;
}
