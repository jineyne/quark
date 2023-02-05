#include "TestUtility.h"
#include "DynLibTestModule.h"

#include <Plugin/DynLibManager.h>
#include <Plugin/PluginManager.h>

TEST(PluginTest, cycle) {
    ASSERT_NO_THROW(FDynLibManager::StartUp());
    ASSERT_NO_THROW(FPluginManager::StartUp());

    gPluginManager().loadPlugin(TEXT("quark-test-dynlib"));

    int data = 0;
    ASSERT_NO_THROW(data = FDynLibTestModule::Instance().getData());
    ASSERT_EQ(data, 100);

    ASSERT_NO_THROW(FPluginManager::ShutDown());
    ASSERT_NO_THROW(FDynLibManager::ShutDown());
}