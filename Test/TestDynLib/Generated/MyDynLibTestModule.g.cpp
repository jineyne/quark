
#include "MyDynLibTestModule.h"
#include "Reflection/GeneratedPrerequisites.h"

void FMyDynLibTestModule::StaticRegisterNativeFMyDynLibTestModule() {
}

IMPLEMENT_CLASS(FMyDynLibTestModule);
QClass *Generated_Initializer_Class_FMyDynLibTestModule();
static FInitClassOnStart Generated_InitClassOnStart_Class_FMyDynLibTestModule(&Generated_Initializer_Class_FMyDynLibTestModule, &FMyDynLibTestModule::StaticClass, TEXT("FMyDynLibTestModule"), TEXT("MyDynLibTestModule.h"));

struct Generated_Class_FMyDynLibTestModule_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FMyDynLibTestModule_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FMyDynLibTestModule_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FMyDynLibTestModule_Statics::ClassDesc = {
    TEXT("FMyDynLibTestModule"),
    FMyDynLibTestModule::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FMyDynLibTestModule_Statics::ClassProperties,
    Generated_Class_FMyDynLibTestModule_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FMyDynLibTestModule() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FMyDynLibTestModule_Statics::ClassDesc);
    }
    return instance;
}


// CLASS FMyDynLibTestModule DECLARE END

