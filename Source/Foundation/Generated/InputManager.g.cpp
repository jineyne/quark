
#include "Manager/InputManager.h"
#include "Reflection/GeneratedPrerequisites.h"

void FInputManager::StaticRegisterNativeFInputManager() {
}
IMPLEMENT_CLASS(FInputManager);
QClass *Generated_Initializer_Class_FInputManager();
static FInitClassOnStart Generated_InitClassOnStart_Class_FInputManager(&Generated_Initializer_Class_FInputManager, &FInputManager::StaticClass, TEXT("FInputManager"), TEXT("Input/InputManager.h"));

struct Generated_Class_FInputManager_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FInputManager_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FInputManager_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FInputManager_Statics::ClassDesc = {
    TEXT("FInputManager"),
    FInputManager::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FInputManager_Statics::ClassProperties,
    Generated_Class_FInputManager_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FInputManager() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FInputManager_Statics::ClassDesc);
    }
    return instance;
}
