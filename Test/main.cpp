#include <gtest/gtest.h>

#include "Reflection/Reflection.h"

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    QReflection::Initialize();

    return RUN_ALL_TESTS();
}
