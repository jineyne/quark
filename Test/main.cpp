#include <gtest/gtest.h>

#include "Logging/LogDefines.h"
#include "Reflection/Reflection.h"
#include "Exception/CrashHandler.h"
#include "Exception/Exception.h"

int main(int argc, char *argv[]) {
    ::testing::GTEST_FLAG(catch_exceptions) = false;

    ::testing::InitGoogleTest(&argc, argv);
    QReflection::Initialize();
    FPlatform::Initialize();

    int result = RUN_ALL_TESTS();

    return result;
}
