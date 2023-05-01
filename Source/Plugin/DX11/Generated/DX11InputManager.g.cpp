
#include "Input/DX11InputManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FDX11InputManager::StaticRegisterNativeFDX11InputManager() {
}
IMPLEMENT_CLASS(FDX11InputManager);
QClass *Generated_Initializer_Class_FDX11InputManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FDX11InputManager(&Generated_Initializer_Class_FDX11InputManager, &FDX11InputManager::StaticClass, TEXT("FDX11InputManager"), TEXT("Input/DX11InputManager.h"));

struct Generated_Class_FDX11InputManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FDX11InputManager_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FDX11InputManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FDX11InputManager_Statics::ClassDesc = {
    TEXT("FDX11InputManager"),
    FDX11InputManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FDX11InputManager_Statics::ClassProperties,
    Generated_Class_FDX11InputManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FDX11InputManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FDX11InputManager_Statics::ClassDesc);
    }
    return instance;
}
