#pragma once

#include "UtilityCore.h"
#include "Vector3.h"

class FVector4 {
public:
    static FVector4 Zero;

public:
    float x{ 0 };
    float y{ 0 };
    float z{ 0 };
    float w{ 0 };

public:
    FVector4() = default;
    FVector4(float val) : x(val), y(val), z(val), w(val) {}
    FVector4(FVector3 vec3, float val) : x(vec3.x), y(vec3.y), z(vec3.z), w(val) {}
    FVector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

public:
    static FVector4 Max(const FVector4 &a, const FVector4 &b);
    static FVector4 Min(const FVector4 &a, const FVector4 &b);

public:
    bool operator==(const FVector4 &rhs);
    bool operator!=(const FVector4 &rhs);

    FVector4 operator+(const FVector4 &rhs) const;
    FVector4 operator-(const FVector4 &rhs) const;
    FVector4 operator*(const FVector4 &rhs) const;
    FVector4 operator/(const FVector4 &rhs) const;

    FVector4 &operator+=(const FVector4 &rhs);
    FVector4 &operator-=(const FVector4 &rhs);
    FVector4 &operator*=(const FVector4 &rhs);
    FVector4 &operator/=(const FVector4 &rhs);

    float operator[] (uint32_t i) const;
    float &operator[] (uint32_t i);

public:
    FVector4 &clamp(FVector4 min, FVector4 max);

    float distance(const FVector4 &other) const;
    float distanceSquared(const FVector4 &other) const;

    float dot(const FVector4 &other) const;

    float length() const;
    float lengthSquared() const;

    void normalize();
    FVector4 normalized() const;

    bool equals(const FVector4 &other) const;
};