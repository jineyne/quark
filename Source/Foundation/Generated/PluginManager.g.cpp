
#include "Plugin/PluginManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FPluginManager::StaticRegisterNativeFPluginManager() {
}
IMPLEMENT_CLASS(FPluginManager);
QClass *Generated_Initializer_Class_FPluginManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FPluginManager(&Generated_Initializer_Class_FPluginManager, &FPluginManager::StaticClass, TEXT("FPluginManager"), TEXT("Plugin/PluginManager.h"));

struct Generated_Class_FPluginManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FPluginManager_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FPluginManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FPluginManager_Statics::ClassDesc = {
    TEXT("FPluginManager"),
    FPluginManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FPluginManager_Statics::ClassProperties,
    Generated_Class_FPluginManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FPluginManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FPluginManager_Statics::ClassDesc);
    }
    return instance;
}
