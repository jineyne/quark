#pragma once

#include "UtilityCore.h"
#include "Degree.h"
#include "Vector3.h"

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
    static FMatrix4 Rotate(const FDegree &angleDegree, const FVector3 &axis);

    static FMatrix4 Transpose(const FMatrix4 &val) noexcept;

public:
    // Matrix multiplication
    FMatrix4 operator*(const FMatrix4& mat) const;

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
    void rotate(const FDegree &angleDegree, const FVector3 &axis);

    // Convert the matrix to a string
    FString toString() const;
};