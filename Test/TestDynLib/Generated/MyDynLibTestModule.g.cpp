
#include "MyDynLibTestModule.h"
#include "Reflection/GeneratedPrerequisites.h"

void FMyDynLibTestModule::StaticRegisterNativeFMyDynLibTestModule() {
}

IMPLEMENT_CLASS(FMyDynLibTestModule);
Class *Generated_Initializer_Class_FMyDynLibTestModule();
static InitClassOnStart Generated_InitClassOnStart_Class_FMyDynLibTestModule(&Generated_Initializer_Class_FMyDynLibTestModule, &FMyDynLibTestModule::StaticClass, TEXT("FMyDynLibTestModule"), TEXT("MyDynLibTestModule.h"));

struct Generated_Class_FMyDynLibTestModule_Statics {

    static const TArray<Reflection::MetaDataPairDesc> ClassMetaData;
    static const TArray<Reflection::PropertyDescBase const*> ClassProperties;
    static const Reflection::ClassDesc ClassDesc;
};

const TArray<Reflection::MetaDataPairDesc> Generated_Class_FMyDynLibTestModule_Statics::ClassMetaData = {
};
const TArray<Reflection::PropertyDescBase const*> Generated_Class_FMyDynLibTestModule_Statics::ClassProperties = {
};

const Reflection::ClassDesc Generated_Class_FMyDynLibTestModule_Statics::ClassDesc = {
    TEXT("FMyDynLibTestModule"),
    FMyDynLibTestModule::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FMyDynLibTestModule_Statics::ClassProperties,
    Generated_Class_FMyDynLibTestModule_Statics::ClassMetaData,
};

Class *Generated_Initializer_Class_FMyDynLibTestModule() {
    static Class *instance = nullptr;
    if (!instance) {
        Reflection::CreateClass(instance, Generated_Class_FMyDynLibTestModule_Statics::ClassDesc);
    }
    return instance;
}


// CLASS FMyDynLibTestModule DECLARE END

