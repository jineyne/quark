/*
#include "ModuleTest.h"

void FTestModule::onStartUp() {
}

void FTestModule::onShutDown() {
}

// module allow only 1 cycle per instance
// so test together
TEST(FTestModule, Cycle) {
    ASSERT_ANY_THROW(FTestModule::Instance());
    ASSERT_ANY_THROW(FTestModule::ShutDown());

    ASSERT_NO_THROW(FTestModule::StartUp());
    ASSERT_NO_THROW(FTestModule::Instance());
    ASSERT_NO_THROW(FTestModule::ShutDown());

    ASSERT_ANY_THROW(FTestModule::Instance());
    ASSERT_ANY_THROW(FTestModule::StartUp());
}*/
