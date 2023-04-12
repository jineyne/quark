
#include "Renderer/ParamBlocks.h"
#include "Reflection/GeneratedPrerequisites.h"

void FParamBlockManager::StaticRegisterNativeFParamBlockManager() {
}
IMPLEMENT_CLASS(FParamBlockManager);
QClass *Generated_Initializer_Class_FParamBlockManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FParamBlockManager(&Generated_Initializer_Class_FParamBlockManager, &FParamBlockManager::StaticClass, TEXT("FParamBlockManager"), TEXT("Renderer/ParamBlocks.h"));

struct Generated_Class_FParamBlockManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FParamBlockManager_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FParamBlockManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FParamBlockManager_Statics::ClassDesc = {
    TEXT("FParamBlockManager"),
    FParamBlockManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FParamBlockManager_Statics::ClassProperties,
    Generated_Class_FParamBlockManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FParamBlockManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FParamBlockManager_Statics::ClassDesc);
    }
    return instance;
}
