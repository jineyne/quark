#include "TestUtility.h"

#include <Math/Matrix4.h>
#include <Math/Quaternion.h>
#include <DirectXMath.h>

TEST(QuaternionTest, RollPitchYaw) {
    FQuaternion m = FQuaternion(1, 5, 10);
    DirectX::XMVECTOR q = DirectX::XMQuaternionRotationRollPitchYaw(10, 1, 5); // pitch, yaw, roll
    DirectX::XMFLOAT4 m2;
    DirectX::XMStoreFloat4(&m2, q);

    EXPECT_NEAR(m.x, m2.x, FLT_EPSILON);
    EXPECT_NEAR(m.y, m2.y, FLT_EPSILON);
    EXPECT_NEAR(m.z, m2.z, FLT_EPSILON);
    EXPECT_NEAR(m.w, m2.w, FLT_EPSILON);
}

TEST(QuaternionTest, ToMatrix) {
    FQuaternion q1 = FQuaternion(0, 0, 90);
    DirectX::XMVECTOR q2 = DirectX::XMQuaternionRotationRollPitchYaw(90, 0, 0);  // pitch, yaw, roll

    Matrix4 m1 = Matrix4::Identity();
    m1.rotate(q1);

    DirectX::XMMATRIX xmmatrix = DirectX::XMMatrixRotationQuaternion(q2);

    DirectX::XMFLOAT4X4 m2;
    DirectX::XMStoreFloat4x4(&m2, xmmatrix);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (std::max(m1[i][j], m2(i, j)) - std::min(m1[i][j], m2(i, j)) >= 0.001) {
                LOG(LogTemp, Debug, TEXT("[%ld][%ld] %llf != %llf"), i, j, m1[i][j], m2(i, j));
            }
            EXPECT_NEAR(m1[i][j], m2(i, j), 0.001);
        }
    }
}

TEST(QuaternionTest, Forward) {
    FQuaternion q1 = FQuaternion(1, 0, 0);
    DirectX::XMVECTOR q2 = DirectX::XMQuaternionRotationRollPitchYaw(0, 1, 0);

    DirectX::XMVECTOR v1 = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), q2);

    Vector3 f1 = q1.rotate(Vector3::Forward);
    DirectX::XMFLOAT3 f2;
    DirectX::XMStoreFloat3(&f2, v1);

    EXPECT_NEAR(f1.x, f2.x, FLT_EPSILON);
    EXPECT_NEAR(f1.y, f2.y, FLT_EPSILON);
    EXPECT_NEAR(f1.z, f2.z, FLT_EPSILON);
}

TEST(QuaternionTest, LookAt) {
    Vector3 forward = Vector3(0, 0, 10) - Vector3(0, 0, 0);

    FQuaternion q1 = FQuaternion(0, 0, 0);
    q1.lookRotation(forward, Vector3::Up);
    auto m1 = Matrix4::Rotate(q1);

    Vector3 f1 = q1.rotate(Vector3::Forward);

    DirectX::XMMATRIX m = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0, 0, 0, 0), DirectX::XMVectorSet(0, 0, 10, 0), DirectX::XMVectorSet(0, 1, 0, 0));
    DirectX::XMFLOAT4X4 m2;
    DirectX::XMStoreFloat4x4(&m2, m);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_NEAR(m1[i][j], m2(i, j), 0.0001);
        }
    }
}