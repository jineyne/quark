
// DO NOT MODIFY MANUALLY!

#ifdef Test_ModuleTest_g_h
#error "ModuleTest.g.h already included. missing '#pragma once' in ModuleTest.h"
#endif

#define Test_ModuleTest_g_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Test_Source_ModuleTest_h

#include "Misc/ModuleTest.h"
#include "Reflection/GeneratedPrerequisites.h"


#define Test_Source_ModuleTest_h_11_GENERATED_FUNCTIONS \
private: \
    friend struct Generated_Class_FTestModule_Statics; \
    static void StaticRegisterNativeFTestModule(); \
public: \
    DECLARE_CLASS(FTestModule, TModule, ) \
    DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(FTestModule) \
    DECLARE_SERIALIZER(FTestModule)

#define Test_Source_ModuleTest_h_11_GENERATED_BODY \
        Test_Source_ModuleTest_h_11_GENERATED_FUNCTIONS
