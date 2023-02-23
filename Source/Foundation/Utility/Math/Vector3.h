#pragma once

#include "UtilityCore.h"
#include "Vector3.g.h"

QSTRUCT()
struct DLL_EXPORT FVector3 {
public:
    GENERATED_BODY();

public:
    static FVector3 ZeroVector;

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
    float dot(const FVector3 &v) const;
    FVector3 cross(const FVector3 &v) const;

    float length() const;
    void normalize();
};