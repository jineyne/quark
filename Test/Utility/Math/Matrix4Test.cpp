#include "TestUtility.h"

#include <Math/Matrix4.h>
#include <DirectXMath.h>

TEST(FMatrix4Test, Translate) {
    FMatrix4 m = FMatrix4::Identity();
    FVector3 v(2.0f, 3.0f, 4.0f);
    FMatrix4 expected = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {2.0f, 3.0f, 4.0f, 1.0f}
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

    m.rotate(v, angle);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_NEAR(m[i][j], expected[i][j], std::numeric_limits<float>::epsilon());
        }
    }
}

TEST(FMatrix4Test, Transform) {
    FMatrix4 m = FMatrix4::Transform(FVector3(0, 0, 1), FQuaternion(0, 1, 0), FVector3(1, 1, 1));

    DirectX::XMFLOAT3 position = {0.0f, 0.0f, 1.0f};
    DirectX::XMFLOAT3 rotation = {0, 1, 0};
    DirectX::XMFLOAT3 scale = {1, 1, 1};

    DirectX::XMVECTOR rotationVector = DirectX::XMLoadFloat3(&rotation);
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(rotationVector);

    // 스케일 매트릭스 생성
    DirectX::XMVECTOR scaleVector = DirectX::XMLoadFloat3(&scale);
    DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScalingFromVector(scaleVector);

    // 이동 매트릭스 생성
    DirectX::XMVECTOR positionVector = DirectX::XMLoadFloat3(&position);
    DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslationFromVector(positionVector);

    DirectX::XMMATRIX modelMatrix = scaleMatrix * rotationMatrix * translationMatrix;
    DirectX::XMFLOAT4X4 model;
    XMStoreFloat4x4(&model, modelMatrix);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_NEAR(m[i][j], model(i, j), FLT_EPSILON);
        }
    }
}

TEST(FMatrix4Test, Perspective) {
    float fov = FRadian(45);
    float aspect = 1280.0 / 720.0;
    float near = 0.1f;
    float far = 1000.f;
    FMatrix4 perspective1 = FMatrix4::Perspective(fov, aspect, near, far);
    DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, near, far);

    DirectX::XMFLOAT4X4 perspective2;
    XMStoreFloat4x4(&perspective2, perspectiveMatrix);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_NEAR(perspective1[i][j], perspective2(i, j), 0.0001);
        }
    }
}

TEST(FMatrix4Test, Multiply) {
    float fov = FRadian(45);
    float aspect = 1280.0 / 720.0;
    float near = 0.1f;
    float far = 1000.f;
    FMatrix4 perspective1 = FMatrix4::Perspective(fov, aspect, near, far);

    DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, near, far);
    DirectX::XMFLOAT4X4 perspective2;
    XMStoreFloat4x4(&perspective2, perspectiveMatrix);

    FMatrix4 model1 = FMatrix4::Transform(FVector3(0, 0, 1), FQuaternion(0, 1, 0), FVector3(1, 1, 1));

    DirectX::XMFLOAT3 position = {0.0f, 0.0f, 1.0f};
    DirectX::XMFLOAT3 rotation = {0, 1, 0};
    DirectX::XMFLOAT3 scale = {1, 1, 1};

    DirectX::XMVECTOR rotationVector = DirectX::XMLoadFloat3(&rotation);
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(rotationVector);

    // 스케일 매트릭스 생성
    DirectX::XMVECTOR scaleVector = DirectX::XMLoadFloat3(&scale);
    DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScalingFromVector(scaleVector);

    // 이동 매트릭스 생성
    DirectX::XMVECTOR positionVector = DirectX::XMLoadFloat3(&position);
    DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslationFromVector(positionVector);

    DirectX::XMMATRIX modelMatrix = scaleMatrix * rotationMatrix * translationMatrix;

    FMatrix4 mvp1 = model1 * FMatrix4::Identity() * perspective1;

    DirectX::XMMATRIX modelViewProjectionMatrix = modelMatrix * DirectX::XMMatrixIdentity() * perspectiveMatrix;
    DirectX::XMFLOAT4X4 mvp2;
    XMStoreFloat4x4(&mvp2, modelViewProjectionMatrix);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_NEAR(mvp1[i][j], mvp2(i, j), 0.0001);
        }
    }
}