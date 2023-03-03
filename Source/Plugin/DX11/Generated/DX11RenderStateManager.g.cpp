
#include "Manager/DX11RenderStateManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FDX11RenderStateManager::StaticRegisterNativeFDX11RenderStateManager() {
}
IMPLEMENT_CLASS(FDX11RenderStateManager);
QClass *Generated_Initializer_Class_FDX11RenderStateManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FDX11RenderStateManager(&Generated_Initializer_Class_FDX11RenderStateManager, &FDX11RenderStateManager::StaticClass, TEXT("FDX11RenderStateManager"), TEXT("RenderAPI/DX11RenderStateManager.h"));

struct Generated_Class_FDX11RenderStateManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FDX11RenderStateManager_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FDX11RenderStateManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FDX11RenderStateManager_Statics::ClassDesc = {
    TEXT("FDX11RenderStateManager"),
    FDX11RenderStateManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FDX11RenderStateManager_Statics::ClassProperties,
    Generated_Class_FDX11RenderStateManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FDX11RenderStateManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FDX11RenderStateManager_Statics::ClassDesc);
    }
    return instance;
}
