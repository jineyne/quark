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
    FQuaternion(const Vector3 &axis, Radian angle);
    FQuaternion(const Vector3 &xAxis, const Vector3 &yAxis, const Vector3 &zAxis);
    FQuaternion(float yaw, float roll, float pitch);

public:
    static FQuaternion GetRotationFromTo(const Vector3& from, const Vector3& dest, const Vector3& fallbackAxis = Vector3::ZeroVector);

public:
    void fromAxisAngle(const Vector3 &axis, Radian radian);
    void fromRollPitchYaw(float yaw, float roll, float pitch);

    FQuaternion operator+ (const FQuaternion &rhs) const;
    FQuaternion operator-(const FQuaternion &rhs) const;
    FQuaternion operator*(const FQuaternion &rhs) const;
    Vector3 operator*(const Vector3 &rhs) const;
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
    Vector3 rotate(const Vector3 &vec) const;

    void lookRotation(const Vector3 &forward);
    void lookRotation(const Vector3 &forward, const Vector3 &up);

    float normalize(float tolerance = 1e-04f);
    FQuaternion normalized() const;

    FQuaternion inverse() const;

    Vector3 zAxis() const;
    Vector3 toEulerAngles() const;
};
