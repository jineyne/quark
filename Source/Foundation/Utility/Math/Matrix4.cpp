//
// Created by jiney on 2023-02-22.
//

#include "Matrix4.h"
#include "Misc/StringBuilder.h"
#include "Quaternion.h"
#include "Radian.h"

Matrix4::Matrix4() : m() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            /*if (i == j) {
                m[i][j] = 1.0f;
            } else {*/
                m[i][j] = 0.0f;
            /*}*/
        }
    }
}

Matrix4::Matrix4(float value) {
    m[0][0] = value;
    m[1][1] = value;
    m[2][2] = value;
    m[3][3] = value;
}

Matrix4::Matrix4(const float data[4][4]) {
    memcpy(m, data, sizeof(m));
}

Matrix4::Matrix4(std::initializer_list<float> values) {
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

Matrix4::Matrix4(std::initializer_list<std::initializer_list<float>> values) {
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

Matrix4 Matrix4::Identity() {
    return Matrix4(1.0f);
}

Matrix4 Matrix4::Perspective(float fov, float aspect, float near, float far) {
    Matrix4 mat;
    float tanHalfFOV = std::tan(fov / 2.0f);
    mat.m[0][0] = 1.0f / (aspect * tanHalfFOV);
    mat.m[1][1] = 1.0f / tanHalfFOV;
    mat.m[2][2] = far / (far - near);
    mat.m[2][3] = 1.0f;
    mat.m[3][2] = -(far * near) / (far - near);
    return mat;
}

Matrix4 Matrix4::Orthographic(float left, float right, float bottom, float top, float near, float far) {
    Matrix4 mat;
    mat.m[0][0] = 2.0f / (right - left);
    mat.m[1][1] = 2.0f / (top - bottom);
    mat.m[2][2] = 1.0f / (far - near);
    mat.m[3][0] = -(right + left) / (right - left);
    mat.m[3][1] = -(top + bottom) / (top - bottom);
    mat.m[3][2] = -near / (far - near);
    mat.m[3][3] = 1.0f;
    return mat;
}

Matrix4 Matrix4::Translate(const Vector3 &vec) {
    auto mat = Matrix4::Identity();
    mat.translate(vec);
    return mat;
}

Matrix4 Matrix4::Scale(const Vector3 &vec) {
    auto mat = Matrix4::Identity();
    mat.scale(vec);
    return mat;
}

Matrix4 Matrix4::Rotate(const Vector3 &axis, const Degree &angleDegree) {
    auto mat = Matrix4::Identity();
    mat.rotate(axis, angleDegree);
    return mat;
}

Matrix4 Matrix4::Rotate(const FQuaternion &quat) {
    auto mat = Matrix4::Identity();
    mat.rotate(quat);
    return mat;
}

Matrix4 Matrix4::Transpose(const Matrix4 &m) noexcept {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[j][i] = m[i][j];
        }
    }
    return result;
}

Matrix4 Matrix4::Transform(const Vector3 &position, const FQuaternion &rotation, const Vector3 &scale) {
    Matrix4 pos = Translate(position);
    Matrix4 rot = Rotate(rotation);
    Matrix4 scl = Scale(scale);

    return scl * rot * pos;
}

Matrix4 Matrix4::operator*(const Matrix4 &mat) const {
    Matrix4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] =
                    m[i][0] * mat.m[0][j] + m[i][1] * mat.m[1][j] + m[i][2] * mat.m[2][j] + m[i][3] * mat.m[3][j];
        }
    }
    return result;
}

Vector4 Matrix4::operator*(const Vector4 &vec) const {
    Vector4 prod;
    for (auto row = 0; row < 4; row++) {
        prod[row] = m[row][0] * vec[0] + m[row][1] * vec[1] + m[row][2] * vec[2] + m[row][3] * vec[3];
    }

    return prod;
}

float *Matrix4::operator[](int i) {
    return m[i];
}

const float *Matrix4::operator[](int i) const {
    return m[i];
}

void Matrix4::translate(const Vector3 &vec) {
    Matrix4 translation = Matrix4::Identity();
    translation.m[3][0] = vec.x;
    translation.m[3][1] = vec.y;
    translation.m[3][2] = vec.z;
    *this = *this * translation;
}

void Matrix4::scale(const Vector3 &vec) {
    Matrix4 scale = Matrix4::Identity();
    scale.m[0][0] = vec.x;
    scale.m[1][1] = vec.y;
    scale.m[2][2] = vec.z;
    *this = *this * scale;
}

void Matrix4::rotate(const Vector3 &axis, const Degree &angleDegree) {
    float angle = Radian(angleDegree);

    float c = std::cos(angle);
    float s = std::sin(angle);

    Vector3 normalizedAxis = axis.normalized();
    float x = normalizedAxis.x;
    float y = normalizedAxis.y;
    float z = normalizedAxis.z;

    Matrix4 rotation = Matrix4::Identity();
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

void Matrix4::rotate(const FQuaternion &quat) {
    float x = quat.x;
    float y = quat.y;
    float z = quat.z;
    float w = quat.w;

    float xx = x * x;
    float xy = x * y;
    float xz = x * z;
    float xw = x * w;

    float yy = y * y;
    float yz = y * z;
    float yw = y * w;

    float zz = z * z;
    float zw = z * w;

    Matrix4 rotation(1);
    /*rotation[0][0] = 1 - 2 * (yy + zz);
    rotation[0][1] = 2 * (xy + zw);
    rotation[0][2] = 2 * (xy - yw);

    rotation[1][0] = 2 * (xy - zw);
    rotation[1][1] = 1 - 2 * (xx + zz);
    rotation[1][2] = 2 * (yz + xw);

    rotation[2][0] = 2 * (xy + yw);
    rotation[2][1] = 2 * (yz - xw);
    rotation[2][2] = 1 - 2 * (xx + yy);*/

    rotation[0][0] = 1.f - 2.f * yy - 2.f * zz;
    rotation[0][1] = 2.f * xy + 2.f * zw;
    rotation[0][2] = 2.f * xz - 2.f * yw;
    rotation[0][3] = 0.f;

    rotation[1][0] = 2.f * xy - 2.f * zw;
    rotation[1][1] = 1.f - 2.f * xx - 2.f * zz;
    rotation[1][2] = 2.f * yz + 2.f * xw;
    rotation[1][3] = 0.f;

    rotation[2][0] = 2.f * xz + 2.f * yw;
    rotation[2][1] = 2.f * yz - 2.f * xw;
    rotation[2][2] = 1.f - 2.f * xx - 2.f * yy;
    rotation[2][3] = 0.f;

    rotation[3][0] = 0.f;
    rotation[3][1] = 0.f;
    rotation[3][2] = 0.f;
    rotation[3][3] = 1.0f;

    *this = *this * rotation;
}

String Matrix4::toString() const {
    FStringBuilder sb(512);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            sb.appendF(TEXT("%lf "), m[i][j]);
        }
        sb.append(TEXT("\n"), 1);
    }

    return sb.toString();
}
