
#include "Misc/Time.h"
#include "Reflection/GeneratedPrerequisites.h"

void FTime::StaticRegisterNativeFTime() {
}
IMPLEMENT_CLASS(FTime);
QClass *Generated_Initializer_Class_FTime();
static FInitClassOnStart Generated_InitClassOnStart_Class_FTime(&Generated_Initializer_Class_FTime, &FTime::StaticClass, TEXT("FTime"), TEXT("Misc/Time.h"));

struct Generated_Class_FTime_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FTime_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FTime_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FTime_Statics::ClassDesc = {
    TEXT("FTime"),
    FTime::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FTime_Statics::ClassProperties,
    Generated_Class_FTime_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FTime() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FTime_Statics::ClassDesc);
    }
    return instance;
}
