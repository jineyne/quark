#include "TestUtility.h"

#include <Math/Vector3.h>

TEST(Vector3Test, Addition) {
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 c = a + b;
    EXPECT_EQ(c.x, 5.0f);
    EXPECT_EQ(c.y, 7.0f);
    EXPECT_EQ(c.z, 9.0f);
}

TEST(Vector3Test, Subtraction) {
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 c = a - b;
    EXPECT_EQ(c.x, -3.0f);
    EXPECT_EQ(c.y, -3.0f);
    EXPECT_EQ(c.z, -3.0f);
}

TEST(Vector3Test, Multiplication) {
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 c = a * b;
    EXPECT_EQ(c.x, 4.0f);
    EXPECT_EQ(c.y, 10.0f);
    EXPECT_EQ(c.z, 18.0f);
}

TEST(Vector3Test, Division) {
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 c = a / b;
    EXPECT_EQ(c.x, 1.0f / 4.0f);
    EXPECT_EQ(c.y, 2.0f / 5.0f);
    EXPECT_EQ(c.z, 3.0f / 6.0f);
}

TEST(Vector3Test, ScalarMultiplication) {
    Vector3 a(1.0f, 2.0f, 3.0f);
    float b = 2.0f;
    Vector3 c = a * b;
    EXPECT_EQ(c.x, 2.0f);
    EXPECT_EQ(c.y, 4.0f);
    EXPECT_EQ(c.z, 6.0f);
}

TEST(Vector3Test, ScalarDivision) {
    Vector3 a(1.0f, 2.0f, 3.0f);
    float b = 2.0f;
    Vector3 c = a / b;
    EXPECT_EQ(c.x, 1.0f / 2.0f);
    EXPECT_EQ(c.y, 2.0f / 2.0f);
    EXPECT_EQ(c.z, 3.0f / 2.0f);
}

TEST(Vector3Test, Length) {
    Vector3 a(1.0f, 2.0f, 2.0f);
    float length = a.length();
    EXPECT_FLOAT_EQ(length, 3.0f);
}

TEST(Vector3Test, DotProduct) {
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    float dot = a.dot(b);
    EXPECT_FLOAT_EQ(dot, 32.0f);
}