
#include "RenderAPI/BufferManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FBufferManager::StaticRegisterNativeFBufferManager() {
}
IMPLEMENT_CLASS_NO_CTR(FBufferManager);
QClass *Generated_Initializer_Class_FBufferManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FBufferManager(&Generated_Initializer_Class_FBufferManager, &FBufferManager::StaticClass, TEXT("FBufferManager"), TEXT("RenderAPI/BufferManager.h"));

struct Generated_Class_FBufferManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FBufferManager_Statics::ClassMetaData = {
    {TEXT("abstract"), TEXT("true")},
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FBufferManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FBufferManager_Statics::ClassDesc = {
    TEXT("FBufferManager"),
    FBufferManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FBufferManager_Statics::ClassProperties,
    Generated_Class_FBufferManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FBufferManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FBufferManager_Statics::ClassDesc);
    }
    return instance;
}
