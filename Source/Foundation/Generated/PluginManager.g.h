
// DO NOT MODIFY MANUALLY!

#ifdef _PluginManager_g_h
#error "PluginManager.g.h already included. missing '#pragma once' in PluginManager.h"
#endif

#define _PluginManager_g_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Source_PluginManager_h

#include "Plugin/PluginManager.h"
#include "Reflection/GeneratedPrerequisites.h"


#define Source_PluginManager_h_23_GENERATED_FUNCTIONS \
private: \
    friend struct Generated_Class_FPluginManager_Statics; \
    static void StaticRegisterNativeFPluginManager(); \
public: \
    DECLARE_CLASS(FPluginManager, TModule, ) \
    DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(FPluginManager) \
    DECLARE_SERIALIZER(FPluginManager)

#define Source_PluginManager_h_23_GENERATED_BODY \
        Source_PluginManager_h_23_GENERATED_FUNCTIONS
