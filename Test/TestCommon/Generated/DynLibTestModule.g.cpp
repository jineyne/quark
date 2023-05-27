
#include "DynLibTestModule.h"
#include "Reflection/GeneratedPrerequisites.h"

void FDynLibTestModule::StaticRegisterNativeFDynLibTestModule() {
}

IMPLEMENT_CLASS(FDynLibTestModule);
Class *Generated_Initializer_Class_FDynLibTestModule();
static InitClassOnStart Generated_InitClassOnStart_Class_FDynLibTestModule(&Generated_Initializer_Class_FDynLibTestModule, &FDynLibTestModule::StaticClass, TEXT("FDynLibTestModule"), TEXT("DynLibTestModule.h"));

struct Generated_Class_FDynLibTestModule_Statics {

    static const TArray<Reflection::MetaDataPairDesc> ClassMetaData;
    static const TArray<Reflection::PropertyDescBase const*> ClassProperties;
    static const Reflection::ClassDesc ClassDesc;
};

const TArray<Reflection::MetaDataPairDesc> Generated_Class_FDynLibTestModule_Statics::ClassMetaData = {
    {TEXT("abstract"), TEXT("")},
};
const TArray<Reflection::PropertyDescBase const*> Generated_Class_FDynLibTestModule_Statics::ClassProperties = {
};

const Reflection::ClassDesc Generated_Class_FDynLibTestModule_Statics::ClassDesc = {
    TEXT("FDynLibTestModule"),
    FDynLibTestModule::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FDynLibTestModule_Statics::ClassProperties,
    Generated_Class_FDynLibTestModule_Statics::ClassMetaData,
};

Class *Generated_Initializer_Class_FDynLibTestModule() {
    static Class *instance = nullptr;
    if (!instance) {
        Reflection::CreateClass(instance, Generated_Class_FDynLibTestModule_Statics::ClassDesc);
    }
    return instance;
}


// CLASS FDynLibTestModule DECLARE END

