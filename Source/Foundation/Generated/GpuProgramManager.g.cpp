
#include "Manager/GpuProgramManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FGpuProgramManager::StaticRegisterNativeFGpuProgramManager() {
}
IMPLEMENT_CLASS_NO_CTR(FGpuProgramManager);
QClass *Generated_Initializer_Class_FGpuProgramManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FGpuProgramManager(&Generated_Initializer_Class_FGpuProgramManager, &FGpuProgramManager::StaticClass, TEXT("FGpuProgramManager"), TEXT("Manager/GpuProgramManager.h"));

struct Generated_Class_FGpuProgramManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FGpuProgramManager_Statics::ClassMetaData = {
    {TEXT("abstract"), TEXT("true")},
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FGpuProgramManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FGpuProgramManager_Statics::ClassDesc = {
    TEXT("FGpuProgramManager"),
    FGpuProgramManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FGpuProgramManager_Statics::ClassProperties,
    Generated_Class_FGpuProgramManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FGpuProgramManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FGpuProgramManager_Statics::ClassDesc);
    }
    return instance;
}
