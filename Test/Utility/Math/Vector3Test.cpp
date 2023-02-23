#include "TestUtility.h"

#include <Math/Vector3.h>

TEST(Vector3Test, Addition) {
    FVector3 a(1.0f, 2.0f, 3.0f);
    FVector3 b(4.0f, 5.0f, 6.0f);
    FVector3 c = a + b;
    EXPECT_EQ(c.x, 5.0f);
    EXPECT_EQ(c.y, 7.0f);
    EXPECT_EQ(c.z, 9.0f);
}

TEST(Vector3Test, Subtraction) {
    FVector3 a(1.0f, 2.0f, 3.0f);
    FVector3 b(4.0f, 5.0f, 6.0f);
    FVector3 c = a - b;
    EXPECT_EQ(c.x, -3.0f);
    EXPECT_EQ(c.y, -3.0f);
    EXPECT_EQ(c.z, -3.0f);
}

TEST(Vector3Test, Multiplication) {
    FVector3 a(1.0f, 2.0f, 3.0f);
    FVector3 b(4.0f, 5.0f, 6.0f);
    FVector3 c = a * b;
    EXPECT_EQ(c.x, 4.0f);
    EXPECT_EQ(c.y, 10.0f);
    EXPECT_EQ(c.z, 18.0f);
}

TEST(Vector3Test, Division) {
    FVector3 a(1.0f, 2.0f, 3.0f);
    FVector3 b(4.0f, 5.0f, 6.0f);
    FVector3 c = a / b;
    EXPECT_EQ(c.x, 1.0f / 4.0f);
    EXPECT_EQ(c.y, 2.0f / 5.0f);
    EXPECT_EQ(c.z, 3.0f / 6.0f);
}

TEST(Vector3Test, ScalarMultiplication) {
    FVector3 a(1.0f, 2.0f, 3.0f);
    float b = 2.0f;
    FVector3 c = a * b;
    EXPECT_EQ(c.x, 2.0f);
    EXPECT_EQ(c.y, 4.0f);
    EXPECT_EQ(c.z, 6.0f);
}

TEST(Vector3Test, ScalarDivision) {
    FVector3 a(1.0f, 2.0f, 3.0f);
    float b = 2.0f;
    FVector3 c = a / b;
    EXPECT_EQ(c.x, 1.0f / 2.0f);
    EXPECT_EQ(c.y, 2.0f / 2.0f);
    EXPECT_EQ(c.z, 3.0f / 2.0f);
}

TEST(Vector3Test, Length) {
    FVector3 a(1.0f, 2.0f, 2.0f);
    float length = a.length();
    EXPECT_FLOAT_EQ(length, 3.0f);
}

TEST(Vector3Test, DotProduct) {
    FVector3 a(1.0f, 2.0f, 3.0f);
    FVector3 b(4.0f, 5.0f, 6.0f);
    float dot = a.dot(b);
    EXPECT_FLOAT_EQ(dot, 32.0f);
}