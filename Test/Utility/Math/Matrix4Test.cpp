#include "TestUtility.h"

#include <Math/Matrix4.h>

TEST(FMatrix4Test, Translate) {
    FMatrix4 m = FMatrix4::Identity();
    FVector3 v(2.0f, 3.0f, 4.0f);
    FMatrix4 expected = {
            {1.0f, 0.0f, 0.0f, 2.0f},
            {0.0f, 1.0f, 0.0f, 3.0f},
            {0.0f, 0.0f, 1.0f, 4.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
    };

    m.translate(v);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_FLOAT_EQ(m[i][j], expected[i][j]);
        }
    }
}

TEST(FMatrix4Test, Scale) {
    FMatrix4 m = FMatrix4::Identity();
    FVector3 v(2.0f, 3.0f, 4.0f);
    FMatrix4 expected = {
            {2.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 3.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 4.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
    };

    m.scale(v);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_FLOAT_EQ(m[i][j], expected[i][j]);
        }
    }
}

TEST(FMatrix4Test, Rotate) {
    FMatrix4 m = FMatrix4::Identity();
    FVector3 v(1.0f, 0.0f, 0.0f);
    float angle = 90.0f;
    FMatrix4 expected = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, -1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
    };

    m.rotate(angle, v);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_NEAR(m[i][j], expected[i][j], std::numeric_limits<float>::epsilon());
        }
    }
}