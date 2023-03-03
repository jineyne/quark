
#include "Manager/DX11RenderWindowManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FDX11RenderWindowManager::StaticRegisterNativeFDX11RenderWindowManager() {
}
IMPLEMENT_CLASS(FDX11RenderWindowManager);
QClass *Generated_Initializer_Class_FDX11RenderWindowManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FDX11RenderWindowManager(&Generated_Initializer_Class_FDX11RenderWindowManager, &FDX11RenderWindowManager::StaticClass, TEXT("FDX11RenderWindowManager"), TEXT("RenderAPI/DX11RenderWindowManager.h"));

struct Generated_Class_FDX11RenderWindowManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FDX11RenderWindowManager_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FDX11RenderWindowManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FDX11RenderWindowManager_Statics::ClassDesc = {
    TEXT("FDX11RenderWindowManager"),
    FDX11RenderWindowManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FDX11RenderWindowManager_Statics::ClassProperties,
    Generated_Class_FDX11RenderWindowManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FDX11RenderWindowManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FDX11RenderWindowManager_Statics::ClassDesc);
    }
    return instance;
}
