
#include "DynLibTestModule.h"
#include "Reflection/GeneratedPrerequisites.h"

void FDynLibTestModule::StaticRegisterNativeFDynLibTestModule() {
}

IMPLEMENT_CLASS(FDynLibTestModule);
QClass *Generated_Initializer_Class_FDynLibTestModule();
static FInitClassOnStart Generated_InitClassOnStart_Class_FDynLibTestModule(&Generated_Initializer_Class_FDynLibTestModule, &FDynLibTestModule::StaticClass, TEXT("FDynLibTestModule"), TEXT("DynLibTestModule.h"));

struct Generated_Class_FDynLibTestModule_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FDynLibTestModule_Statics::ClassMetaData = {
    {TEXT("abstract"), TEXT("")},
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FDynLibTestModule_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FDynLibTestModule_Statics::ClassDesc = {
    TEXT("FDynLibTestModule"),
    FDynLibTestModule::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FDynLibTestModule_Statics::ClassProperties,
    Generated_Class_FDynLibTestModule_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FDynLibTestModule() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FDynLibTestModule_Statics::ClassDesc);
    }
    return instance;
}


// CLASS FDynLibTestModule DECLARE END

