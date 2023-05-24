#pragma once

#include "UtilityCore.h"
#include "Vector3.h"
#include "Radian.h"
#include "Quaternion.g.h"

QSTRUCT()
struct DLL_EXPORT FQuaternion {
    GENERATED_BODY()

public:
    static FQuaternion Zero;

public:
    QPROPERTY()
    float x;

    QPROPERTY()
    float y;

    QPROPERTY()
    float z;

    QPROPERTY()
    float w;

public:
    FQuaternion(float value = 0.0f);
    FQuaternion(float x, float y, float z, float w);
    FQuaternion(const FVector3 &axis, FRadian angle);
    FQuaternion(const FVector3 &xAxis, const FVector3 &yAxis, const FVector3 &zAxis);
    FQuaternion(float yaw, float roll, float pitch);

public:
    static FQuaternion GetRotationFromTo(const FVector3& from, const FVector3& dest, const FVector3& fallbackAxis = FVector3::ZeroVector);

public:
    void fromAxisAngle(const FVector3 &axis, FRadian radian);
    void fromRollPitchYaw(float yaw, float roll, float pitch);

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

    void lookRotation(const FVector3 &forward);
    void lookRotation(const FVector3 &forward, const FVector3 &up);

    float normalize(float tolerance = 1e-04f);
    FQuaternion normalized() const;

    FQuaternion inverse() const;

    FVector3 zAxis() const;
    FVector3 toEulerAngles() const;
};
