
#include "RenderAPI/DX11RenderAPI.h"
#include "Reflection/GeneratedPrerequisites.h"

void FDX11RenderAPI::StaticRegisterNativeFDX11RenderAPI() {
}
IMPLEMENT_CLASS(FDX11RenderAPI);
QClass *Generated_Initializer_Class_FDX11RenderAPI();
static FInitClassOnStart Generated_InitClassOnStart_Class_FDX11RenderAPI(&Generated_Initializer_Class_FDX11RenderAPI, &FDX11RenderAPI::StaticClass, TEXT("FDX11RenderAPI"), TEXT("RenderAPI/DX11RenderAPI.h"));

struct Generated_Class_FDX11RenderAPI_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FDX11RenderAPI_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FDX11RenderAPI_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FDX11RenderAPI_Statics::ClassDesc = {
    TEXT("FDX11RenderAPI"),
    FDX11RenderAPI::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FDX11RenderAPI_Statics::ClassProperties,
    Generated_Class_FDX11RenderAPI_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FDX11RenderAPI() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FDX11RenderAPI_Statics::ClassDesc);
    }
    return instance;
}
