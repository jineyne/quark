#pragma once

#include "UtilityCore.h"
#include "Vector2.g.h"

QSTRUCT()
struct DLL_EXPORT Vector2 {
    GENERATED_BODY()

public:
    QPROPERTY()
    float x;

    QPROPERTY()
    float y;

public:
    constexpr Vector2() : x(0), y(0) {}
    constexpr Vector2(float x, float y) : x(x), y(y) {}

public:
    // Linear interpolation between two vectors
    static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);

    // Component-wise minimum value of two vectors
    static Vector2 Min(const Vector2& a, const Vector2& b);

    // Component-wise maximum value of two vectors
    static Vector2 Max(const Vector2& a, const Vector2& b);

    // Distance between two vectors
    static float Distance(const Vector2& a, const Vector2& b);

public:
    // Operators
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;

    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;

    // dot product
    float dot(const Vector2& other) const;

    // cross product (2D)
    float cross(const Vector2& other) const;

    // length (length)
    float length() const;

    // Normalization (unit vector)
    Vector2 normalized() const;

    // angle between two vectors
    float angle(const Vector2& other) const;

    // rotate vector by an angle (in radians)
    Vector2 rotate(float angle) const;

    float distance(const Vector2 &other) const;
};