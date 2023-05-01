#pragma once

#include "UtilityCore.h"
#include "Vector2.g.h"

QSTRUCT()
struct DLL_EXPORT FVector2 {
    GENERATED_BODY()

public:
    QPROPERTY()
    float x;

    QPROPERTY()
    float y;

public:
    // Constructors
    constexpr FVector2() : x(0), y(0) {}
    constexpr FVector2(float x, float y) : x(x), y(y) {}

public:
    // Linear interpolation between two vectors
    static FVector2 Lerp(const FVector2& start, const FVector2& end, float t);

    // Component-wise minimum value of two vectors
    static FVector2 Min(const FVector2& a, const FVector2& b);

    // Component-wise maximum value of two vectors
    static FVector2 Max(const FVector2& a, const FVector2& b);

    // Distance between two vectors
    static float Distance(const FVector2& a, const FVector2& b);

public:
    // Operators
    FVector2 operator+(const FVector2& other) const;
    FVector2 operator-(const FVector2& other) const;
    FVector2 operator*(float scalar) const;
    FVector2 operator/(float scalar) const;

    bool operator==(const FVector2& other) const;
    bool operator!=(const FVector2& other) const;

    // dot product
    float dot(const FVector2& other) const;

    // cross product (2D)
    float cross(const FVector2& other) const;

    // length (length)
    float length() const;

    // Normalization (unit vector)
    FVector2 normalized() const;

    // angle between two vectors
    float angle(const FVector2& other) const;

    // rotate vector by an angle (in radians)
    FVector2 rotate(float angle) const;

    float distance(const FVector2 &other) const;
};