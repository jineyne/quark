//
// Created by jiney on 2023-02-22.
//

#include "Matrix4.h"
#include "Misc/StringBuilder.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Radian.h"

FMatrix4::FMatrix4() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                m[i][j] = 1.0f;
            } else {
                m[i][j] = 0.0f;
            }
        }
    }
}

FMatrix4::FMatrix4(float value) {
    m[0][0] = value;
    m[1][1] = value;
    m[2][2] = value;
    m[3][3] = value;
}

FMatrix4::FMatrix4(const float data[4][4]) {
    memcpy(m, data, sizeof(m));
}

FMatrix4::FMatrix4(std::initializer_list<float> values) {
    int i = 0, j = 0;
    for (auto it = values.begin(); it != values.end(); it++) {
        m[i][j] = *it;
        j++;
        if (j == 4) {
            j = 0;
            i++;
        }
        if (i == 4) break;
    }
}

FMatrix4::FMatrix4(std::initializer_list<std::initializer_list<float>> values) {
    int i = 0, j = 0;
    for (auto row = values.begin(); row != values.end(); row++) {
        j = 0;
        for (auto it = row->begin(); it != row->end(); it++) {
            m[i][j] = *it;
            j++;
            if (j == 4) break;
        }
        i++;
        if (i == 4) break;
    }
}

FMatrix4 FMatrix4::Identity() {
    return FMatrix4(1.0f);
}

FMatrix4 FMatrix4::Perspective(float fov, float aspect, float near, float far) {
    FMatrix4 mat;
    float tanHalfFOV = std::tan(fov / 2.0f);
    mat.m[0][0] = 1.0f / (aspect * tanHalfFOV);
    mat.m[1][1] = 1.0f / tanHalfFOV;
    mat.m[2][2] = far / (far - near);
    mat.m[2][3] = 1.0f;
    mat.m[3][2] = -(far * near) / (far - near);
    return mat;
}

FMatrix4 FMatrix4::Orthographic(float left, float right, float bottom, float top, float near, float far) {
    FMatrix4 mat;
    mat.m[0][0] = 2.0f / (right - left);
    mat.m[1][1] = 2.0f / (top - bottom);
    mat.m[2][2] = 1.0f / (far - near);
    mat.m[3][0] = -(right + left) / (right - left);
    mat.m[3][1] = -(top + bottom) / (top - bottom);
    mat.m[3][2] = -near / (far - near);
    mat.m[3][3] = 1.0f;
    return mat;
}

FMatrix4 FMatrix4::operator*(const FMatrix4 &mat) const {
    FMatrix4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] =
                    m[i][0] * mat.m[0][j] + m[i][1] * mat.m[1][j] + m[i][2] * mat.m[2][j] + m[i][3] * mat.m[3][j];
        }
    }
    return result;
}

float *FMatrix4::operator[](int i) {
    return m[i];
}

const float *FMatrix4::operator[](int i) const {
    return m[i];
}

void FMatrix4::translate(const FVector3 &vec) {
    FMatrix4 translation;
    translation.m[0][3] = vec.x;
    translation.m[1][3] = vec.y;
    translation.m[2][3] = vec.z;
    *this = *this * translation;
}

void FMatrix4::scale(const FVector3 &vec) {
    FMatrix4 scale;
    scale.m[0][0] = vec.x;
    scale.m[1][1] = vec.y;
    scale.m[2][2] = vec.z;
    *this = *this * scale;
}

void FMatrix4::rotate(const FDegree &angleDegree, const FVector3 &axis) {
    float angle = FRadian(angleDegree);

    float c = std::cos(angle);
    float s = std::sin(angle);

    FVector3 normalizedAxis = axis;
    normalizedAxis.normalize();
    float x = normalizedAxis.x;
    float y = normalizedAxis.y;
    float z = normalizedAxis.z;

    FMatrix4 rotation;
    rotation.m[0][0] = c + (1 - c) * x * x;
    rotation.m[0][1] = (1 - c) * x * y - s * z;
    rotation.m[0][2] = (1 - c) * x * z + s * y;
    rotation.m[1][0] = (1 - c) * y * x + s * z;
    rotation.m[1][1] = c + (1 - c) * y * y;
    rotation.m[1][2] = (1 - c) * y * z - s * x;
    rotation.m[2][0] = (1 - c) * z * x - s * y;
    rotation.m[2][1] = (1 - c) * z * y + s * x;
    rotation.m[2][2] = c + (1 - c) * z * z;

    *this = *this * rotation;
}

FString FMatrix4::toString() const {
    FStringBuilder sb(512);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            sb.appendF(TEXT("%lf "), m[i][j]);
        }
        sb.append(TEXT("\n"), 1);
    }

    return sb.toString();
}
