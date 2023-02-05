
#include "Misc/ModuleTest.h"
#include "Reflection/GeneratedPrerequisites.h"

void FTestModule::StaticRegisterNativeFTestModule() {
    // FUNCTION onStartUp DECLARE
    // FUNCTION onShutDown DECLARE
}

IMPLEMENT_CLASS(FTestModule);
QClass *Generated_Initializer_Class_FTestModule();
static FInitClassOnStart Generated_InitClassOnStart_Class_FTestModule(&Generated_Initializer_Class_FTestModule, &FTestModule::StaticClass, TEXT("FTestModule"), TEXT("Misc/ModuleTest.h"));

struct Generated_Class_FTestModule_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FTestModule_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FTestModule_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FTestModule_Statics::ClassDesc = {
    TEXT("FTestModule"),
    FTestModule::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FTestModule_Statics::ClassProperties,
    Generated_Class_FTestModule_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FTestModule() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FTestModule_Statics::ClassDesc);
    }
    return instance;
}


// CLASS FTestModule DECLARE END

