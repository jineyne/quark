
#include "Manager/DX11CommandBufferManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FDX11CommandBufferManager::StaticRegisterNativeFDX11CommandBufferManager() {
}
IMPLEMENT_CLASS(FDX11CommandBufferManager);
QClass *Generated_Initializer_Class_FDX11CommandBufferManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FDX11CommandBufferManager(&Generated_Initializer_Class_FDX11CommandBufferManager, &FDX11CommandBufferManager::StaticClass, TEXT("FDX11CommandBufferManager"), TEXT("RenderAPI/DX11CommandBufferManager.h"));

struct Generated_Class_FDX11CommandBufferManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FDX11CommandBufferManager_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FDX11CommandBufferManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FDX11CommandBufferManager_Statics::ClassDesc = {
    TEXT("FDX11CommandBufferManager"),
    FDX11CommandBufferManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FDX11CommandBufferManager_Statics::ClassProperties,
    Generated_Class_FDX11CommandBufferManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FDX11CommandBufferManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FDX11CommandBufferManager_Statics::ClassDesc);
    }
    return instance;
}
