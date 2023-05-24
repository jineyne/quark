#pragma once

#include "UtilityCore.h"
#include "Vector3.g.h"

QSTRUCT()
struct DLL_EXPORT FVector3 {
public:
    GENERATED_BODY();

public:
    static FVector3 ZeroVector;
    static FVector3 Forward;
    static FVector3 Right;
    static FVector3 Up;

public:
    QPROPERTY()
    float x = 0.f;

    QPROPERTY()
    float y = 0.f;

    QPROPERTY()
    float z = 0.f;

public:
    FVector3();
    FVector3(float x, float y, float z);

public:
    static float Dot(const FVector3& a, const FVector3& b);

    // Linear interpolation between two vectors
    static FVector3 Lerp(const FVector3& start, const FVector3& end, float t);

    // Component-wise minimum value of two vectors
    static FVector3 Min(const FVector3& a, const FVector3& b);

    // Component-wise maximum value of two vectors
    static FVector3 Max(const FVector3& a, const FVector3& b);

    static FVector3 Cross(const FVector3 &a, const FVector3 &b);

    // Distance between two vectors
    static float Distance(const FVector3& a, const FVector3& b);

    // Squared distance between two vectors (faster than Distance)
    static float DistanceSquared(const FVector3& a, const FVector3& b);

public:
    FVector3 operator+(const FVector3 &rhs) const;
    FVector3 operator-(const FVector3 &rhs) const;
    FVector3 operator*(const FVector3 &rhs) const;
    FVector3 operator/(const FVector3 &rhs) const;

    FVector3 operator*(float v) const;
    FVector3 operator/(float v) const;

    FVector3 &operator+=(const FVector3 &rhs);
    FVector3 &operator-=(const FVector3 &rhs);
    FVector3 &operator*=(const FVector3 &rhs);
    FVector3 &operator/=(const FVector3 &rhs);

    FVector3 operator-() const;

    bool operator==(const FVector3 &rhs) const;
    bool operator!=(const FVector3 &rhs) const;

public:
    // dot product
    float dot(const FVector3& other) const;

    // cross product
    FVector3 cross(const FVector3& other) const;

    // length (length)
    float length() const;

    float lengthSquared() const;

    // Normalization (unit vector)
    FVector3 normalized() const;

    // angle between two vectors
    float angle(const FVector3& other) const;

    // rotate vector by an angle (in radians) around an axis
    FVector3 rotate(float angle, const FVector3& axis) const;

    // distance between two vectors
    float distance(const FVector3 &other) const;


};