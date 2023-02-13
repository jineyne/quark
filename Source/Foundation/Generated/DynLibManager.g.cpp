
#include "Plugin/DynLibManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FDynLibManager::StaticRegisterNativeFDynLibManager() {
}

IMPLEMENT_CLASS(FDynLibManager);
QClass *Generated_Initializer_Class_FDynLibManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FDynLibManager(&Generated_Initializer_Class_FDynLibManager, &FDynLibManager::StaticClass, TEXT("FDynLibManager"), TEXT("Plugin/DynLibManager.h"));

struct Generated_Class_FDynLibManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FDynLibManager_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FDynLibManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FDynLibManager_Statics::ClassDesc = {
    TEXT("FDynLibManager"),
    FDynLibManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FDynLibManager_Statics::ClassProperties,
    Generated_Class_FDynLibManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FDynLibManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FDynLibManager_Statics::ClassDesc);
    }
    return instance;
}


// CLASS FDynLibManager DECLARE END

