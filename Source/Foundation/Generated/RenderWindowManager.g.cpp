
#include "RenderAPI/RenderWindowManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FRenderWindowManager::StaticRegisterNativeFRenderWindowManager() {
}
IMPLEMENT_CLASS_NO_CTR(FRenderWindowManager);
QClass *Generated_Initializer_Class_FRenderWindowManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FRenderWindowManager(&Generated_Initializer_Class_FRenderWindowManager, &FRenderWindowManager::StaticClass, TEXT("FRenderWindowManager"), TEXT("RenderAPI/RenderWindowManager.h"));

struct Generated_Class_FRenderWindowManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FRenderWindowManager_Statics::ClassMetaData = {
    {TEXT("abstract"), TEXT("true")},
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FRenderWindowManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FRenderWindowManager_Statics::ClassDesc = {
    TEXT("FRenderWindowManager"),
    FRenderWindowManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FRenderWindowManager_Statics::ClassProperties,
    Generated_Class_FRenderWindowManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FRenderWindowManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FRenderWindowManager_Statics::ClassDesc);
    }
    return instance;
}
