#pragma once

#include "UtilityCore.h"
#include "Vector3.g.h"

QSTRUCT()
struct DLL_EXPORT Vector3 {
public:
    GENERATED_BODY();

public:
    static Vector3 ZeroVector;
    static Vector3 Forward;
    static Vector3 Right;
    static Vector3 Up;

public:
    QPROPERTY()
    float x = 0.f;

    QPROPERTY()
    float y = 0.f;

    QPROPERTY()
    float z = 0.f;

public:
    Vector3();
    Vector3(float x, float y, float z);

public:
    static float Dot(const Vector3& a, const Vector3& b);

    // Linear interpolation between two vectors
    static Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

    // Component-wise minimum value of two vectors
    static Vector3 Min(const Vector3& a, const Vector3& b);

    // Component-wise maximum value of two vectors
    static Vector3 Max(const Vector3& a, const Vector3& b);

    static Vector3 Cross(const Vector3 &a, const Vector3 &b);

    // Distance between two vectors
    static float Distance(const Vector3& a, const Vector3& b);

    // Squared distance between two vectors (faster than Distance)
    static float DistanceSquared(const Vector3& a, const Vector3& b);

public:
    Vector3 operator+(const Vector3 &rhs) const;
    Vector3 operator-(const Vector3 &rhs) const;
    Vector3 operator*(const Vector3 &rhs) const;
    Vector3 operator/(const Vector3 &rhs) const;

    Vector3 operator*(float v) const;
    Vector3 operator/(float v) const;

    Vector3 &operator+=(const Vector3 &rhs);
    Vector3 &operator-=(const Vector3 &rhs);
    Vector3 &operator*=(const Vector3 &rhs);
    Vector3 &operator/=(const Vector3 &rhs);

    Vector3 operator-() const;

    bool operator==(const Vector3 &rhs) const;
    bool operator!=(const Vector3 &rhs) const;

public:
    // dot product
    float dot(const Vector3& other) const;

    // cross product
    Vector3 cross(const Vector3& other) const;

    // length (length)
    float length() const;

    float lengthSquared() const;

    // Normalization (unit vector)
    Vector3 normalized() const;

    // angle between two vectors
    float angle(const Vector3& other) const;

    // rotate vector by an angle (in radians) around an axis
    Vector3 rotate(float angle, const Vector3& axis) const;

    // distance between two vectors
    float distance(const Vector3 &other) const;


};