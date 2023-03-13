
#include "Manager/DX11GpuProgramManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FDX11GpuProgramManager::StaticRegisterNativeFDX11GpuProgramManager() {
}
IMPLEMENT_CLASS(FDX11GpuProgramManager);
QClass *Generated_Initializer_Class_FDX11GpuProgramManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FDX11GpuProgramManager(&Generated_Initializer_Class_FDX11GpuProgramManager, &FDX11GpuProgramManager::StaticClass, TEXT("FDX11GpuProgramManager"), TEXT("Manager/DX11GpuProgramManager.h"));

struct Generated_Class_FDX11GpuProgramManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FDX11GpuProgramManager_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FDX11GpuProgramManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FDX11GpuProgramManager_Statics::ClassDesc = {
    TEXT("FDX11GpuProgramManager"),
    FDX11GpuProgramManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FDX11GpuProgramManager_Statics::ClassProperties,
    Generated_Class_FDX11GpuProgramManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FDX11GpuProgramManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FDX11GpuProgramManager_Statics::ClassDesc);
    }
    return instance;
}
