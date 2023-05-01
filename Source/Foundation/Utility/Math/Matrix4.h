#pragma once

#include "UtilityCore.h"
#include "Degree.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

class DLL_EXPORT FMatrix4 {
private:
    float m[4][4] = {0, };

public:
    // Default constructor that initializes the matrix to the Identity matrix
    FMatrix4();

    // Constructor that initializes elements to the given mValue
    FMatrix4(float value);

    // Constructor that initializes the matrix with a 2D array
    FMatrix4(const float data[4][4]);

    // Constructor that initializes the matrix with an initializer_list
    FMatrix4(std::initializer_list<float> values);

    // Constructor that takes an initializer_list to initialize the matrix
    FMatrix4(std::initializer_list<std::initializer_list<float>> values);

public:
    // Returns an Identity matrix
    static FMatrix4 Identity();

    // Create a Perspective projection matrix
    static FMatrix4 Perspective(float fov, float aspect, float near, float far);

    // Create an Orthographic projection matrix
    static FMatrix4 Orthographic(float left, float right, float bottom, float top, float near, float far);

    // Create an Translate matrix
    static FMatrix4 Translate(const FVector3 &vec);

    // Create an Scale matrix
    static FMatrix4 Scale(const FVector3 &vec);

    // Create an Rotate matrix
    static FMatrix4 Rotate(const FVector3 &axis, const FDegree &angleDegree);
    static FMatrix4 Rotate(const FQuaternion &quat);

    static FMatrix4 Transpose(const FMatrix4 &val) noexcept;

    static FMatrix4 Transform(const FVector3 &position, const FQuaternion &rotation, const FVector3 &scale);

public:
    // Matrix multiplication
    FMatrix4 operator*(const FMatrix4& mat) const;
    FVector4 operator*(const FVector4 &vec) const;

    // Access individual elements of the matrix
    float* operator[](int i);

    // Access individual elements of the matrix
    const float* operator[](int i) const;

public:
    // Translate the matrix by a vector
    void translate(const FVector3& vec);

    // Scale the matrix by a vector
    void scale(const FVector3& vec);

    // rotate the matrix around an axis
    void rotate(const FVector3 &axis, const FDegree &angleDegree);
    void rotate(const FQuaternion &quat);

    // Convert the matrix to a string
    FString toString() const;
};