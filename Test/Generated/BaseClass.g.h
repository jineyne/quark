
// DO NOT MODIFY MANUALLY!

#ifdef Test_BaseClass_g_h
#error "BaseClass.g.h already included. missing '#pragma once' in BaseClass.h"
#endif

#define Test_BaseClass_g_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Test_Source_BaseClass_h

#include "Reflection/GeneratedPrerequisites.h"


#define Test_Source_BaseClass_h_10_GENERATED_FUNCTIONS \
private: \
    friend struct Generated_Class_FBaseClass_Statics; \
    static void StaticRegisterNativeFBaseClass(); \
public: \
    DECLARE_CLASS(FBaseClass, QObject, ) \
    DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(FBaseClass) \
    DECLARE_SERIALIZER(FBaseClass)

#define Test_Source_BaseClass_h_10_GENERATED_BODY \
        Test_Source_BaseClass_h_10_GENERATED_FUNCTIONS
