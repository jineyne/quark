
#include "Manager/RenderStateManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FRenderStateManager::StaticRegisterNativeFRenderStateManager() {
}
IMPLEMENT_CLASS_NO_CTR(FRenderStateManager);
QClass *Generated_Initializer_Class_FRenderStateManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FRenderStateManager(&Generated_Initializer_Class_FRenderStateManager, &FRenderStateManager::StaticClass, TEXT("FRenderStateManager"), TEXT("Manager/RenderStateManager.h"));

struct Generated_Class_FRenderStateManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FRenderStateManager_Statics::ClassMetaData = {
    {TEXT("abstract"), TEXT("true")},
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FRenderStateManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FRenderStateManager_Statics::ClassDesc = {
    TEXT("FRenderStateManager"),
    FRenderStateManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FRenderStateManager_Statics::ClassProperties,
    Generated_Class_FRenderStateManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FRenderStateManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FRenderStateManager_Statics::ClassDesc);
    }
    return instance;
}
