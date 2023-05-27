#pragma once

#include "UtilityCore.h"
#include "Vector3.h"
#include "Vector4.g.h"

QSTRUCT()
struct Vector4 {
    GENERATED_BODY()
public:
    static Vector4 Zero;

public:
    QPROPERTY()
    float x = 0;

    QPROPERTY()
    float y = 0;

    QPROPERTY()
    float z = 0;

    QPROPERTY()
    float w = 0;

public:
    Vector4() = default;
    Vector4(float val) : x(val), y(val), z(val), w(val) {}
    Vector4(Vector3 vec3, float val) : x(vec3.x), y(vec3.y), z(vec3.z), w(val) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

public:
    static Vector4 Max(const Vector4 &a, const Vector4 &b);
    static Vector4 Min(const Vector4 &a, const Vector4 &b);

public:
    bool operator==(const Vector4 &rhs);
    bool operator!=(const Vector4 &rhs);

    Vector4 operator+(const Vector4 &rhs) const;
    Vector4 operator-(const Vector4 &rhs) const;
    Vector4 operator*(const Vector4 &rhs) const;
    Vector4 operator/(const Vector4 &rhs) const;

    Vector4 &operator+=(const Vector4 &rhs);
    Vector4 &operator-=(const Vector4 &rhs);
    Vector4 &operator*=(const Vector4 &rhs);
    Vector4 &operator/=(const Vector4 &rhs);

    float operator[] (uint32_t i) const;
    float &operator[] (uint32_t i);

public:
    Vector4 &clamp(Vector4 min, Vector4 max);

    float distance(const Vector4 &other) const;
    float distanceSquared(const Vector4 &other) const;

    float dot(const Vector4 &other) const;

    float length() const;
    float lengthSquared() const;

    void normalize();
    Vector4 normalized() const;

    bool equals(const Vector4 &other) const;
};