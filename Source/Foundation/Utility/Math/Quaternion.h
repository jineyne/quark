#pragma once

#include "UtilityCore.h"
#include "Vector3.h"
#include "Radian.h"

QCLASS()
struct DLL_EXPORT FQuaternion {
public:
    static FQuaternion Zero;

public:
    float x, y, z, w;

public:
    FQuaternion(float value = 0.0f);
    FQuaternion(float x, float y, float z, float w);
    FQuaternion(const FVector3 &axis, FRadian angle);
    FQuaternion(FRadian xAngle, FRadian yAngle, FRadian zAngle);

public:
    void fromAxisAngle(const FVector3 &axis, FRadian radian);
    void fromEulerAngles(FRadian xAngle, FRadian yAngle, FRadian zAngle);

    FQuaternion operator+ (const FQuaternion &rhs) const;
    FQuaternion operator-(const FQuaternion &rhs) const;
    FQuaternion operator*(const FQuaternion &rhs) const;
    FVector3 operator*(const FVector3 &rhs) const;
    FQuaternion operator*(float rhs) const;
    FQuaternion operator/(float rhs) const;
    FQuaternion operator-() const;

    bool operator==(const FQuaternion &rhs) const;
    bool operator!=(const FQuaternion &rhs) const;

    FQuaternion &operator+=(const FQuaternion &rhs);
    FQuaternion &operator-=(const FQuaternion &rhs);
    FQuaternion &operator*=(const FQuaternion &rhs);

    friend FQuaternion operator*(float lhs, const FQuaternion &rhs);

public:
    float dot(const FQuaternion &quat) const;
    FVector3 rotate(const FVector3 &vec) const;

    float normalize(float tolerance = 1e-04f);

    FQuaternion inverse() const;

    FVector3 toEulerAngles() const;
};
