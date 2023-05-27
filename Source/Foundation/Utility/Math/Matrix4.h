#pragma once

#include "UtilityCore.h"
#include "Degree.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

class DLL_EXPORT Matrix4 {
private:
    float m[4][4] = {0, };

public:
    // Default constructor that initializes the matrix to the Identity matrix
    Matrix4();

    // Constructor that initializes elements to the given mValue
    Matrix4(float value);

    // Constructor that initializes the matrix with a 2D array
    Matrix4(const float data[4][4]);

    // Constructor that initializes the matrix with an initializer_list
    Matrix4(std::initializer_list<float> values);

    // Constructor that takes an initializer_list to initialize the matrix
    Matrix4(std::initializer_list<std::initializer_list<float>> values);

public:
    // Returns an Identity matrix
    static Matrix4 Identity();

    // Create a Perspective projection matrix
    static Matrix4 Perspective(float fov, float aspect, float near, float far);

    // Create an Orthographic projection matrix
    static Matrix4 Orthographic(float left, float right, float bottom, float top, float near, float far);

    // Create an Translate matrix
    static Matrix4 Translate(const Vector3 &vec);

    // Create an Scale matrix
    static Matrix4 Scale(const Vector3 &vec);

    // Create an Rotate matrix
    static Matrix4 Rotate(const Vector3 &axis, const Degree &angleDegree);
    static Matrix4 Rotate(const FQuaternion &quat);

    static Matrix4 Transpose(const Matrix4 &val) noexcept;

    static Matrix4 Transform(const Vector3 &position, const FQuaternion &rotation, const Vector3 &scale);

public:
    // Matrix multiplication
    Matrix4 operator*(const Matrix4& mat) const;
    Vector4 operator*(const Vector4 &vec) const;

    // Access individual elements of the matrix
    float* operator[](int i);

    // Access individual elements of the matrix
    const float* operator[](int i) const;

public:
    // Translate the matrix by a vector
    void translate(const Vector3& vec);

    // Scale the matrix by a vector
    void scale(const Vector3& vec);

    // rotate the matrix around an axis
    void rotate(const Vector3 &axis, const Degree &angleDegree);
    void rotate(const FQuaternion &quat);

    // Convert the matrix to a string
    String toString() const;
};