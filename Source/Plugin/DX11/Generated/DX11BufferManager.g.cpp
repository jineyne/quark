
#include "Manager/DX11BufferManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FDX11BufferManager::StaticRegisterNativeFDX11BufferManager() {
}
IMPLEMENT_CLASS(FDX11BufferManager)
QClass *Generated_Initializer_Class_FDX11BufferManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FDX11BufferManager(&Generated_Initializer_Class_FDX11BufferManager, &FDX11BufferManager::StaticClass, TEXT("FDX11BufferManager"), TEXT("RenderAPI/DX11BufferManager.h"));

struct Generated_Class_FDX11BufferManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FDX11BufferManager_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FDX11BufferManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FDX11BufferManager_Statics::ClassDesc = {
    TEXT("FDX11BufferManager"),
    FDX11BufferManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FDX11BufferManager_Statics::ClassProperties,
    Generated_Class_FDX11BufferManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FDX11BufferManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FDX11BufferManager_Statics::ClassDesc);
    }
    return instance;
}
