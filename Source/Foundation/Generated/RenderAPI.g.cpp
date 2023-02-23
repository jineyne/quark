
#include "RenderAPI/RenderAPI.h"
#include "Reflection/GeneratedPrerequisites.h"

void FRenderAPI::StaticRegisterNativeFRenderAPI() {
}
IMPLEMENT_CLASS_NO_CTR(FRenderAPI);
QClass *Generated_Initializer_Class_FRenderAPI();
static FInitClassOnStart Generated_InitClassOnStart_Class_FRenderAPI(&Generated_Initializer_Class_FRenderAPI, &FRenderAPI::StaticClass, TEXT("FRenderAPI"), TEXT("RenderAPI/RenderAPI.h"));

struct Generated_Class_FRenderAPI_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FRenderAPI_Statics::ClassMetaData = {
    {TEXT("abstract"), TEXT("true")},
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FRenderAPI_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FRenderAPI_Statics::ClassDesc = {
    TEXT("FRenderAPI"),
    FRenderAPI::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FRenderAPI_Statics::ClassProperties,
    Generated_Class_FRenderAPI_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FRenderAPI() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FRenderAPI_Statics::ClassDesc);
    }
    return instance;
}
