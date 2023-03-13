
#include "Manager/CommandBufferManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FCommandBufferManager::StaticRegisterNativeFCommandBufferManager() {
}
IMPLEMENT_CLASS_NO_CTR(FCommandBufferManager);
QClass *Generated_Initializer_Class_FCommandBufferManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FCommandBufferManager(&Generated_Initializer_Class_FCommandBufferManager, &FCommandBufferManager::StaticClass, TEXT("FCommandBufferManager"), TEXT("Manager/CommandBufferManager.h"));

struct Generated_Class_FCommandBufferManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FCommandBufferManager_Statics::ClassMetaData = {
    {TEXT("abstract"), TEXT("true")},
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FCommandBufferManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FCommandBufferManager_Statics::ClassDesc = {
    TEXT("FCommandBufferManager"),
    FCommandBufferManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FCommandBufferManager_Statics::ClassProperties,
    Generated_Class_FCommandBufferManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FCommandBufferManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FCommandBufferManager_Statics::ClassDesc);
    }
    return instance;
}
