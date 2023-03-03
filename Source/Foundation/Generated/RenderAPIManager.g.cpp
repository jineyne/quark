
#include "Manager/RenderAPIManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FRenderAPIManager::StaticRegisterNativeFRenderAPIManager() {
}
IMPLEMENT_CLASS(FRenderAPIManager);
QClass *Generated_Initializer_Class_FRenderAPIManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FRenderAPIManager(&Generated_Initializer_Class_FRenderAPIManager, &FRenderAPIManager::StaticClass, TEXT("FRenderAPIManager"), TEXT("RenderAPI/RenderAPIManager.h"));

struct Generated_Class_FRenderAPIManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FRenderAPIManager_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FRenderAPIManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FRenderAPIManager_Statics::ClassDesc = {
    TEXT("FRenderAPIManager"),
    FRenderAPIManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FRenderAPIManager_Statics::ClassProperties,
    Generated_Class_FRenderAPIManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FRenderAPIManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FRenderAPIManager_Statics::ClassDesc);
    }
    return instance;
}
