#include "TestUtility.h"

#include <Math/Vector3.h>

TEST(Vector3Test, add) {
    FVector3 add = FVector3(1, 1, 1) + FVector3(2, 2, 2);
    ASSERT_EQ(add.x, 3);
    ASSERT_EQ(add.y, 3);
    ASSERT_EQ(add.z, 3);
}

TEST(Vector3Test, sub) {
    FVector3 sub = FVector3(3, 3, 3) - FVector3(2, 2, 2);
    ASSERT_EQ(sub.x, 1);
    ASSERT_EQ(sub.y, 1);
    ASSERT_EQ(sub.z, 1);
}

TEST(Vector3Test, mul) {
    FVector3 mul = FVector3(3, 3, 3) * FVector3(2, 2, 2);
    ASSERT_EQ(mul.x, 6);
    ASSERT_EQ(mul.y, 6);
    ASSERT_EQ(mul.z, 6);

    FVector3 literal = FVector3(5, 5, 5) * 2;
    ASSERT_EQ(literal.x, 10);
    ASSERT_EQ(literal.y, 10);
    ASSERT_EQ(literal.z, 10);
}

TEST(Vector3Test, div) {
    FVector3 div = FVector3(6, 6, 6) / FVector3(2, 2, 2);
    ASSERT_EQ(div.x, 3);
    ASSERT_EQ(div.y, 3);
    ASSERT_EQ(div.z, 3);

    FVector3 literal = FVector3(10, 10, 10) / 2;
    ASSERT_EQ(literal.x, 5);
    ASSERT_EQ(literal.y, 5);
    ASSERT_EQ(literal.z, 5);
}